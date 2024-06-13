/* Ejemplo con threads en Linux. Compilar con: gcc -o p p.c -lpthread */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <fcntl.h>
#include <unistd.h>

#define BMP_FILE "input.bmp"
#define GRAYSCALE_FILE "output_grayscale.bmp"

#pragma pack(push, 1)
typedef struct
{
    unsigned char magic[2];
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offset;
} BMPHeader;

typedef struct
{
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

unsigned char *imagen_original;              // guarda la direccion de memoria de la imagen
unsigned char nueva_imagen[2000 * 2000 * 3]; // donde vamos a guarda la nueva imagen antes de pasarla al archivo nuevo
int width, height, padding;
BMPInfoHeader infoh; // encabezado

void convertir(int inicio, int fin)
{
    for (int y = inicio; y < inicio + fin; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // pixel es una estructura de 3 (rojo,verde,azul)
            unsigned char *pixel = &imagen_original[(y * (width * 3 + padding)) + (x * 3)]; // se usa para obtiene  un píxel específico de del arrelgo de la imagen original
            unsigned char gray = (unsigned char)(0.3 * pixel[2] + 0.59 * pixel[1] + 0.11 * pixel[0]);
            nueva_imagen[(y * (width * 3 + padding)) + (x * 3)] = gray;
            nueva_imagen[(y * (width * 3 + padding)) + (x * 3) + 1] = gray;
            nueva_imagen[(y * (width * 3 + padding)) + (x * 3) + 2] = gray;
        }
    }
}

void *thread(void *arg)
{
    int thread_id = *((int *)arg);
    int filas_por_hilo = height / 3;
    int inicio = thread_id * filas_por_hilo;
    int fin;
    if (thread_id == 2)
    {
        fin = height - inicio; // para obtener la cantidad de filas totales por si no es multiplo de tres en el ultimo hilo
    }
    else
    {
        fin = filas_por_hilo;
    }

    convertir(inicio, fin);
}

void something_wrong(int fd, const char *m)
{
    if (fd > 0)
        close(fd);
    printf("%s\n", m);
    exit(1);
}

int main()
{
    int in_fd = open(BMP_FILE, O_RDONLY);
    if (in_fd < 0)
    {
        something_wrong(in_fd, "Error open");
    }

    BMPHeader h;
    read(in_fd, &h, sizeof(BMPHeader));

    if (h.magic[0] != 'B' || h.magic[1] != 'M')
    {
        something_wrong(in_fd, "No es BMP");
    }

    read(in_fd, &infoh, sizeof(BMPInfoHeader));

    if (infoh.bpp != 24 || infoh.compression != 0)
    {
        something_wrong(in_fd, "No 24-bit BMP");
    }

    width = infoh.width;
    height = infoh.height;
    padding = (4 - (width * 3) % 4) % 4;

    // Asignar memoria para la imagen original y la nueva imagen
    int img_size = (width * 3 + padding) * height;
    imagen_original = (unsigned char *)malloc(img_size); // asignamos memoria dinamica
    // imagen_original[img_size];
    /*
    SEEK_SET: El desplazamiento se cuenta desde el inicio del archivo.
    SEEK_CUR: El desplazamiento se cuenta desde la posición actual del puntero.
    SEEK_END: El desplazamiento se cuenta desde el final del archivo
    */
    lseek(in_fd, h.offset, SEEK_SET);
    read(in_fd, imagen_original, img_size); // guardamos la imagen en imagen_original
                                            // volatile unsigned char image [img_size];
    int out_fd = open(GRAYSCALE_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out_fd < 0)
    {
        something_wrong(in_fd, "Error open output");
    }

    // Escribir los encabezados BMP en el archivo de salida
    write(out_fd, &h, sizeof(BMPHeader));
    write(out_fd, &infoh, sizeof(BMPInfoHeader));

    pthread_t threads[3];
    int thread_ids[3] = {0, 1, 2};

    // Crear hilos
    for (int i = 0; i < 3; i++)
    {
        pthread_create(&threads[i], NULL, thread, &thread_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < 3; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Escribir la imagen convertida a escala de grises
    lseek(out_fd, h.offset, SEEK_SET); // movemos el puntero despues del encabezado
    write(out_fd, nueva_imagen, img_size);

    close(in_fd);
    close(out_fd);

    // Liberar memoria
    free(imagen_original); // liberamos la memoria dinamica

    printf("Imagen en escala de grises generada: %s\n", GRAYSCALE_FILE);
    return 0;
}
