#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"
#define SHM_NAME "/shm_image"

#pragma pack(push, 1)
typedef struct {
	unsigned char magic[2];
	unsigned int size;
	unsigned short reserved1;
	unsigned short reserved2;
	unsigned int offset;
} BMPHeader;

typedef struct {
	unsigned int headerSize;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int imageSize;
	int xPixelsPerM;
	int yPixelsPerM;
	unsigned int colorsUsed;
	unsigned int colorsImportant;
} BMPInfoHeader;
#pragma pack(pop)

void convertir(int in_fd, unsigned char *nueva_imagen, BMPHeader h, BMPInfoHeader infoh, int inicio, int fin) {
	int width = infoh.width;
	int height = infoh.height;
	unsigned char gray;

	int padding = (4 - (width * 3) % 4) % 4; // Calculando el padding
	unsigned char pixel[3];

	lseek(in_fd, h.offset + inicio * (width * 3 + padding), SEEK_SET); // Mover el puntero al inicio de la sección

	for (int y = inicio; y < fin; y++) {
		for (int x = 0; x < width; x++) {
			read(in_fd, pixel, 3);
			gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
			nueva_imagen[y * (width * 3 + padding) + x * 3] = gray;
			nueva_imagen[y * (width * 3 + padding) + x * 3 + 1] = gray;
			nueva_imagen[y * (width * 3 + padding) + x * 3 + 2] = gray;
		}
		lseek(in_fd, padding, SEEK_CUR); // Saltar el padding
	}
}

void something_wrong(int fd, const char *m) {
	if (fd > 0) {
		close(fd);
	}
	printf("%s\n", m);
	exit(1);
}

int main() {
	int in_fd = open(BMP_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error abriendo el archivo de entrada");
	}

	BMPHeader h;
	if (read(in_fd, &h, sizeof(BMPHeader)) != sizeof(BMPHeader)) {
		something_wrong(in_fd, "Error leyendo el encabezado BMP");
	}

	if (h.magic[0] != 'B' || h.magic[1] != 'M') {
		something_wrong(in_fd, "No es un archivo BMP");
	}

	BMPInfoHeader infoh;
	if (read(in_fd, &infoh, sizeof(BMPInfoHeader)) != sizeof(BMPInfoHeader)) {
		something_wrong(in_fd, "Error leyendo el encabezado de información BMP");
	}

	if (infoh.bpp != 24 || infoh.compression != 0) {
		something_wrong(in_fd, "No es un BMP de 24 bits sin compresión");
	}

	// Crear memoria compartida
	int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1) {
		something_wrong(shm_fd, "Error creando la memoria compartida");
		exit(-1);
	}

	int imageSize = 2000*2000*3;
	if (ftruncate(shm_fd, imageSize) == -1) { // ftruncate sirve para cambiar el tamaño del segmento comapartido
		something_wrong(shm_fd, "Error configurando el tamaño de la memoria compartida");
	}

	// Mapear la memoria compartida al puntero nueva_imagen
	unsigned char *nueva_imagen = mmap(0, imageSize, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (nueva_imagen == MAP_FAILED) {
		something_wrong(shm_fd, "Error mapeando la memoria compartida");
		exit(-1);
	}

	int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (out_fd < 0) {
		something_wrong(in_fd, "Error abriendo el archivo de salida");
	}

	int pid, pid2, pid3;
	pid = fork();

	if (pid == 0) {
		int in_fd_child = open(BMP_FILE, O_RDONLY);
		convertir(in_fd_child, nueva_imagen, h, infoh, 0, 354);
		close(in_fd_child);
		exit(0);
	} else {
		pid2 = fork();
		if (pid2 == 0) {
			int in_fd_child = open(BMP_FILE, O_RDONLY);
			convertir(in_fd_child, nueva_imagen, h, infoh, 354, 709);
			close(in_fd_child);
			exit(0);
		} else {
			pid3 = fork();
			if (pid3 == 0) {
				int in_fd_child = open(BMP_FILE, O_RDONLY);
				convertir(in_fd_child, nueva_imagen, h, infoh, 709, 1063);
				close(in_fd_child);
				exit(0);
			} else {
				waitpid(pid, NULL, 0);
				waitpid(pid2, NULL, 0);
				waitpid(pid3, NULL, 0);

				write(out_fd, &h, sizeof(BMPHeader));
				write(out_fd, &infoh, sizeof(BMPInfoHeader));
				write(out_fd, &nueva_imagen[0], infoh.width * infoh.height * 3);

				close(in_fd);
				close(out_fd);

				// Cerrar y eliminar la memoria compartida
				if (shm_unlink(SHM_NAME) == -1) {
					something_wrong(shm_fd, "Error eliminando la memoria compartida");
				}

				close(shm_fd);
				printf("Imagen en gris generada: %s\n", GRAYSCALE_FILE);
				printf("Proceso padre: %d, hijos terminaron.\n", getpid());
			}
		}
	}
	return 0;
}
