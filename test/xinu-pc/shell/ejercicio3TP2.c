
/* mut.c - mut, operar, incrementar */
#include <xinu.h>
void operar(), incrementar(); //mutex_lock(void), mutex_unlock(void);
unsigned char x = 0;
int pid_1,pid_2,pid=-1;
/*------------------------------------------------------------------------
* mut -- programa con regiones criticas
*------------------------------------------------------------------------
*/

void ejercicio3TP2(void){
    int i;
    sid32 lock;
    lock = semcreate(1);
    pid_1 = create(operar, 8000, 20, "process 1", 1,&lock);
    pid_2 = create(incrementar, 8000, 20, "process 2", 1,&lock);
    resume( pid_1);
    resume( pid_2);
    sleep(10);
}
/*------------------------------------------------------------------------
* operar x e y
*------------------------------------------------------------------------
*/
void operar(sid32 *lock){
    int y = 0;
    printf("Si no existen mensajes de ERROR entonces todo va OK! \n");
    while (1) {
    /* si x es multiplo de 10 */
        mutex_lock(lock);
        if ((x % 10) == 0) {
            y = x * 2;
            /* como y es el doble de x entonces y es multiplo de 10 tambien*/         
            /* si y no es multiplo de 10 entonces hubo un error */
            if ((y % 10) != 0)
            printf("\r ERROR!! y=%d, x=%d \r", y, x);
        }
        mutex_unlock(lock);
    }
}
/*------------------------------------------------------------------------
* incrementar x
*------------------------------------------------------------------------
*/
void incrementar(sid32 *lock){
    while (1) {
        mutex_lock(lock);
        x = x + 1;
        mutex_unlock(lock);
    }
}

void mutex_lock(sid32 *lock){
    wait(lock);
    if(pid == -1){
        pid = getpid();
    }
}

void mutex_unlock(sid32 *lock){
    if(pid == getpid()){
        pid = -1;
        signal(lock);
    }else{
        printf("ERROR \n");
    }
}
