#include <xinu.h>
//matar a todos despues de 5 seg
procesoA(),procesoB(),procesoC(),matar();
sid32 lock2;
int pid1=-1;
void ejercicio1TP2(void){
    int pid_a,pid_b,pid_c,pid_stop;
    int a,b,c;
    a = 0;
    b = 0;
    c = 0;
    lock2 = semcreate(1);
    //& y * indica que el parametro es por referencia
    pid_a = create(procesoA,2048,10,"proceso_a",1,&a);
    pid_b = create(procesoB,2048,10,"proceso_b",1,&b);
    pid_c = create(procesoC,2048,10,"proceso_c",1,&c);
    pid_stop = create(matar,1024,21,"proceso_stop",3,pid_a,pid_b,pid_c);
    //pone el la cola de listos
    //suspend(pid)
    resume(pid_stop);
    resume(pid_a);
    resume(pid_b);
    resume(pid_c);

    high_level_scheduler(pid_a,120,pid_b,60,pid_c,20,200);
    putc(CONSOLE,a);
    putc(CONSOLE,b);
    putc(CONSOLE,c);
}

void high_level_scheduler (int pid_a,int ms_a,int pid_b, int ms_b,int pid_c, int ms_c,int ms){
    int pid,pri,pri_a,pri_b,pri_c;

    pid = getpid();                           
    pri = getprio(pid);  
    pri_a = getprio(pid_a);                       
    pri_b = getprio(pid_b);
    pri_c = getprio(pid_c);

    while (1) {               
        //que pasa si el quantum termina primero                   
        chprio(pid_a,pri-1);                   
        sleepms(ms_a);                                            
        chprio(pid_a,pri_a);  

        chprio(pid_b,pri-1);                    
        sleepms(ms_b);                                            
        chprio(pid_b,pri_b);  

        chprio(pid_c,pri-1);                    
        sleepms(ms_c);                                            
        chprio(pid_c,pri_c);        
    }
}

procesoA(int *a){
    while(1){
        printf("A");
        a++;
    }
}

procesoB(int *b){
    while(1){    
        printf("B");
        b++;
    }
}

procesoC(int *c){
    while(1){
        printf("C");
        c++;
    }
}
//quiero que el proceso stop mate los procesos a,b,c despues de 5seg
matar(int pid_a,int pid_b, int pid_c){
    sleepms(300);
    kill(pid_a);
    kill(pid_b);
    kill(pid_c);
}

void m (void){
    wait(lock2);
    if(pid1 == -1){
        pid1 = getpid();
    }   
}

void um(void){
    if(pid1== getpid()){
        pid1 = -1;
        signal(lock2);
    }
}