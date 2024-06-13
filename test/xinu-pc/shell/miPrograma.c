
/* ex2.c - main, sndA, sndB */

#include <xinu.h>

void	sndA(void), sndB(void);

/*------------------------------------------------------------------------
 * main  --  example of creating processes in Xinu
 *------------------------------------------------------------------------
 */
void	miPrograma(void)
{	//EL SO coloca el proceso en estado listo
	//Crea un proceso
	int pid1;
	int pid2;

	pid1 = create(sndA, 1024, 20, "process 1", 0);
	resume(pid1);
	sleep(10);
	kill(pid1);
	pid2 = create(sndB, 1024, 20, "process 2", 0);
	resume(pid2);
	sleep(10);
	kill(pid2);
	
}

/*------------------------------------------------------------------------
 * sndA  --  repeatedly emit 'A' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndA(void)
{
	while( 1 )
		putc(CONSOLE, 'A');
		
}

/*------------------------------------------------------------------------
 * sndB  --  repeatedly emit 'B' on the console without terminating
 *------------------------------------------------------------------------
 */
void	sndB(void)
{
	while( 1 )
		putc(CONSOLE, 'B');
		sleep(1000);
}