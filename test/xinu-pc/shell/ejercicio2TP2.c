#include <xinu.h>
void produce(void),consume(void);
int32 n = 0;  
sid32 productor, consumidor;

void ejercicio2TP2(void){
    productor = semcreate(1);
    consumidor = semcreate(0);
    resume(create(consume, 4000, 20, "cons",0));
    resume(create(produce, 4000, 20, "prod",0));
}

void produce(void){
    int32 i;
   
    for(i=1;i<=2000;i++){
        wait(productor);
        n++;
        signal(consumidor);
    }
 
}

void consume(void){
    int32 i;
  
    for(i=1;i<=2000;i++){
        wait(consumidor);
        printf("El valo de n es %d\n",n);
        signal(productor); 
    }
     
}