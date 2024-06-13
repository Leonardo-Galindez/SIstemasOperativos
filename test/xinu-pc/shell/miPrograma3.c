/* Creación y finalización de proceso en XINU */
#include <xinu.h>
void imprimir(void);
void miPrograma3(void)
{
    int pid , i;
    
    pid = create(imprimir, 2048, 20, "process 1", 0);
    resume(pid);
    for (i = 0 ; i<=1000 ; i++){
        putc(CONSOLE,i);
    }
    exit();
    sleep(5);
    kill(pid);
    printf("Mate a mi propio hijo (suena horrible)\n");
}
/* proceso sndA */
void imprimir(void)
{
    int i;
    for (i = 1000 ; i<=5000 ; i++){
        putc(CONSOLE,i);
    }
    exit();
}