#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

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
//problemas con variable compartida
void convertir(int in_fd, int out_fd, BMPInfoHeader infoh, int start, int end) {
    int width = infoh.width;
    unsigned char gray;

    int padding = (4 - (width * 3) % 4) % 4; // Calculating padding(relleno) size
    unsigned char pixel[3];
	//sizeof(BMPHeader) + sizeof(BMPInfoHeader) encabezado hay que movernos para no escribirlo
    lseek(in_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader) + start * (width * 3 + padding), SEEK_SET); // Move to start line

    for (int y = start; y < end; y++) {
        for (int x = 0; x < width; x++) {
            read(in_fd, pixel, 3);
            gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
			//vamos moviendo el puntero pixel por pixel
            lseek(out_fd, sizeof(BMPHeader) + sizeof(BMPInfoHeader) + y * (width * 3 + padding) + x * 3, SEEK_SET);
            write(out_fd, &gray, 1);
            write(out_fd, &gray, 1);
            write(out_fd, &gray, 1);
        }
        lseek(in_fd, padding, SEEK_CUR); // Skipping padding
    }
}

void something_wrong(int fd, const char *m) {
    if (fd > 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main() {
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0) {
        something_wrong(in_fd, "Error open");
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));
    if (h.magic[0] != 'B' || h.magic[1] != 'M') {
        something_wrong(in_fd, "No es BMP");
    }

    BMPInfoHeader infoh;
    read(in_fd, &infoh, sizeof(BMPInfoHeader));
    if (infoh.bpp != 24 || infoh.compression != 0) {
        something_wrong(in_fd, "No 24-bit BMP");
    }

    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0) {
        something_wrong(out_fd, "Error open output");
    }

    // Write BMP headers to the output file
    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));

    close(out_fd);

    int total_rows = infoh.height;
    int rows_per_process = total_rows / 3;

    int pid1 = fork();
    if (pid1 == 0) {
        int in_fd = open(BMP_FILE, O_RDONLY);
        int out_fd = open(GRAYSCALE_FILE, O_WRONLY);
    
        convertir(in_fd, out_fd, infoh, 0, rows_per_process);

        close(in_fd);
        close(out_fd);
        exit(0);
    } else {
        int pid2 = fork();
        if (pid2 == 0) {
            int in_fd = open(BMP_FILE, O_RDONLY);
            int out_fd = open(GRAYSCALE_FILE, O_WRONLY);
            convertir(in_fd, out_fd, infoh, rows_per_process, 2 * rows_per_process);
            close(in_fd);
            close(out_fd);
            exit(0);
        } else {
            int pid3 = fork();
            if (pid3 == 0) {
                int in_fd = open(BMP_FILE, O_RDONLY);
                int out_fd = open(GRAYSCALE_FILE, O_WRONLY);
                convertir(in_fd, out_fd, infoh, 2 * rows_per_process, total_rows);
                close(in_fd);
                close(out_fd);
                exit(0);
            } else {
                wait(NULL);
                wait(NULL);
                wait(NULL);
                printf("Imagen en gris generada: %s\n", GRAYSCALE_FILE);
            }
        }
    }
    close(in_fd);
    return 0;
}

