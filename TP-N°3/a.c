#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
/* productor */
int main()
{
    const int SIZE = 4096;
    const char *name = "OS";
    // const char *file_dr = "/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";
    const char *file_dr = "/home/leonardo/SistemasOperativos/TP-N°3/txt.txt";
    int shm_fd;
    void *ptr;
    int file_fd;
    // 6 lectura y escritura
    // 7 lectura y escritura y ejecucion
    // 421
    // rwx
    // dueño,grupos,otros
    //  Creamos el segmento de memoria compartida

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("Error al crear la memoria compartida");
        return -1;
    }
    // Configuramos el tamaño de la memoria compartida
    // error retorna -1 , exito 0
    ftruncate(shm_fd, SIZE);
    // Asignamos el segmento de memoria compartida en el espacio de direcciones del proceso
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("Map failed");
        return -1;
    }

    file_fd = open(file_dr,  O_RDWR);
    if (file_fd == -1)
    {
        perror("Error al abrir el archivo de texto");
        return 1;
    }

    read(file_fd, ptr, SIZE);


    close(file_fd);
    close(shm_fd);

  
    return 0;
}

  // flags: Modos de apertura (como O_RDONLY, O_WRONLY, O_RDWR, O_CREAT, O_TRUNC, etc.).
    //  consultar sobre el fopen esta bien diferencia entre los 2
    /*FILE *file = fopen(file_dr, "r+");
    if (file == NULL)
    {
        perror("Error al abrir el archivo");
        return 1;
    }

    char buffer[] = "Haola como estas?";
    size_t bytes_written = fwrite(buffer, sizeof(char), sizeof(buffer) - 1, file);
    if (bytes_written > 0)
    {
        printf("Se escribieron %zu bytes en el archivo.\n", bytes_written);
    }
    else
    {
        printf("No se pudo escribir en el archivo.\n");
    }
//
    size_t bytes_read = fread(ptr, 1, SIZE, file);
    if (bytes_read != sizeof(buffer) - 1)
    {
        printf("Error al leer el archivo y colocarlo en la memoria compartida\n");
    }
    //int munmap(void *ptr, size_t SIZE);
    fclose(file);*/
    /* Ahora escribimos en el segmento de memoria compartida */
    /*sprintf(ptr, "%s", mess0);
    ptr += strlen(mess0);
    sprintf(ptr, "%s", mess1);*/