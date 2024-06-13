#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // Para el uso de exit()
#include <sys/types.h>
#include <signal.h>

int esPrimo(int num) {
    int rta = 1;
    int i = 2;

    while (rta == 1 && i < num) {
        if (num % i == 0) {
            rta = 0;
        }
        i++;
    }
    return rta;
}

void imprimir(int inicio, int fin){
    for (int i = inicio; i <= fin; i++) {
        if (esPrimo(i) == 1) {
            printf(" %d ", i);
        }
    }
}

int main(void) {
    int pid;
    int x = 0;
    pid = fork();
    if (pid == 0) {
        printf("INICIO HIJO \n");
        imprimir(1000,5000);
        printf("FIN HIJO \n");
    } else {
        printf("INICIO PADRE \n");
        imprimir(0,1000);
        sleep(1);//dormimos al padre para que puede ejecutarse el hijo
        kill(pid, SIGKILL);
        printf("FIN PADRE \n");
        exit(0);
    }
    return 0;
}

