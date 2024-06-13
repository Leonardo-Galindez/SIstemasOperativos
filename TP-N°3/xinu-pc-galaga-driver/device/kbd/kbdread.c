/* kbdread.c  -  kbdread */

#include <xinu.h>
#include <mouse.h>

extern int pid;
/*------------------------------------------------------------------------
 * kbdread  -  Read the status of the keyboard driver
 *------------------------------------------------------------------------
 */
devcall kbdread(
	struct dentry *devptr, /* Entry in device switch table	*/ //??
	char *buffer,											   /* Address of buffer            */
	uint32 count											   /* Length of buffer             */
)
{
	if (pid == getpid())//verificamos que el proceso que hizo el open sea el mismo que hace el read
	{
		/*for(int i=0; i<count; i++){
			buffer=kbdget();
		}*/
		buffer = kbdget();
		return 0;
	}
}