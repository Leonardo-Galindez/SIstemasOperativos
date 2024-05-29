#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
/* productor */
int main(){
    const int SIZE = 4096;
    const char *name = "OS";
    const char *mess0= "Studying ";
    const char *mess1= "Operating Systems ";
    //const char file_dr = "/usr/share/doc/aufs-dkms/filesystems/aufs/design/06mmap.txt";
    const char *file_dr = "/home/leonardo/SistemasOperativos/TP-N°3/txt.txt";
    int shm_fd;
    void *ptr;

    /* create the shared memory segment */

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        printf("Error al crear la memoria compartida");
        return -1;
    }
    /* configure the size of the shared memory segment */
    ftruncate(shm_fd,SIZE);
    /* now map the shared memory segment in the
    * address space of the process */
    ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        printf("Map failed\n");
        return -1;
    }
    
    // Abrir el archivo de texto
    FILE *file = fopen(file_dr, "r");
    if (!file) {
        perror("Error al abrir el archivo de texto");
        return -1;
    }

    // Leer el contenido del archivo y colocarlo en la memoria compartida
    size_t bytes_read = fread(ptr, 1, SIZE, file);
    if (bytes_read == 0) {
        perror("Error al leer el archivo y escribir en la memoria compartida");
        return -1;
    }
   
    fclose(file);
    return 0;
}
