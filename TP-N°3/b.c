#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
/* consumidor */
int main()
{
    const char *name = "OS";
    const int SIZE = 4096;
    int shm_fd;
    void *ptr;
    int i;

    /* abrimos la memoria el segmento de memoria compartida con modo lectura*/
    shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1)
    {
        printf("shared memory failed\n");
        exit(-1);
    }

    // Asignamos el segmento de memoria compartida en el espacio de direcciones del proceso
    ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED)
    {
        printf("Map failed\n");
        exit(-1);
    }

    /* now read from the shared memory region */
    printf("Contenido de la memoria compartida:\n%s\n", (char *)ptr);
    size_t current_length = strlen(ptr);
    char texto[] = " Mundo";
    if (lseek(shm_fd, current_length, SEEK_SET) == -1)
    {
        perror("Error al mover el cursor del archivo");
        munmap(ptr, SIZE);
        close(shm_fd);
        exit(EXIT_FAILURE);
    }
    write(shm_fd, texto, sizeof(texto));
    printf("Contenido de la memoria compartida:\n%s\n", (char *)ptr);
    // limpiar
    /* remove the shared memory segment */
   
}
