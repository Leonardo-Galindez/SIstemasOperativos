/* kbdget.c  -  kbdget */

#include <xinu.h>
#include <keyboard.h>

extern sid32 semaphore;
extern struct kbd_buffer buffer;
extern int pid;

void kbdget(){	
	unsigned char tecla_pulsada;
     
	wait(semaphore);

	tecla_pulsada = buffer.array[buffer.frente];
	buffer.frente= (buffer.frente+1) % BUFFER_SIZE;
		
	return tecla_pulsada;
	
}