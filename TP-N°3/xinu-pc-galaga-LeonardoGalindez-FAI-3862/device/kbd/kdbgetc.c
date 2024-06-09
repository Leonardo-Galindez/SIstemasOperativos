#include <xinu.h>
#include<keyboard.h>

extern sid32 semaforo;
extern unsigned char tecla_actual;
extern unsigned char buffer;
extern int cabeza;
extern int cola;
extern int pos;

kdbgetc(){
    wait(semaforo);
    buffer[pos] = tecla_actual;
}