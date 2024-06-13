/* primos entre el nÃºmero 1 y MAX */

#include <xinu.h>

#define MAX 500000

void cambiarPrioridad();
int encontrar_primos();
int id = -1;
int total = 0;
sid32 lock;
//main
int primos(){
    int pid_1,pid_2,pid = -1;
    lock = semcreate(1);
    pid_1 =create(encontrar_primos,8000,10,"proceso_1",3,0,5000);
    pid_2 =create(encontrar_primos,8000,10,"proceso_2",3,5001,9999);
    
    resume(pid_1);
    resume(pid_2);
    cambiarPrioridad(pid_1,40,pid_2,160);

    //encontrar_primos(10000, MAX);
    printf("Total : %i ", total);

    return 0;
}

void mutex_lock2(void){
    wait(lock);
    if(id == -1){
        id = getpid();
    }
}

void mutex_unlock2(void){
    if(id == getpid()){
        id = -1;
        signal(lock);
    }
}  

int encontrar_primos(int from, int to){
    int i, n, primo;
    printf( "  " );
    mutex_lock2();
    for ( i = from ; i <= to ; i++ ) {
        primo = 1;
        n = 2;

        while ( n <= i / 2 && primo ) {
            if ( i % n == 0 )
                primo = 0;

            n++;
        }

        if ( primo )
            printf( "%d ", i );
    }
    
    total++;
    mutex_unlock2();
    return 0;
}
//planificador
void cambiarPrioridad(int pid_1,int ms_1,int pid_2,int ms_2){
    int pri_1,pri_2,pri,pid;

    pid = getpid();
    pri = getprio(pid);
    pri_1 = getprio(pid_1);
    pri_2 = getprio(pid_2);

    chprio(pid_1,pid-1); 
    sleepms(ms_1);
    chprio(pid_1,pri_1);

    chprio(pid_2,pid-1); 
    sleepms(ms_2);
    chprio(pid_2,pri_2);
}
 




