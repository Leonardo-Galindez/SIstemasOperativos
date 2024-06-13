/* kbdopen.c  -  kbdopen */

#include <xinu.h>
#include <keyboard.h>

extern int pid;
extern sid32 semaphore;
extern struct kbd_buffer buffer;

/*------------------------------------------------------------------------
 * kbdopen  -  Open the ps/2 keyboard device
 *------------------------------------------------------------------------
 */

devcall	kbdopen (
	 struct	dentry	*devptr,	/* Entry in device switch table	*/
	 char	*name,			/* Unused for a kbd */
	 char	*mode			/* Unused for a kbd */
	)
{
	//reinicia el FIFO
	buffer.frente = 0;
	buffer.fin = 0;
	
	
	if(pid != -1){
		return -1;//hay otro proceso que hizo el open
	}
		
	pid = getpid();


	return 0;
	
}