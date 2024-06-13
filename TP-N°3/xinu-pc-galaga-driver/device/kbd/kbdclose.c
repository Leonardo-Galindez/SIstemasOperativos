/* kbdclose.c  -  kbdclose */

#include <xinu.h>

extern int pid;

/*------------------------------------------------------------------------
 * kbdclose  -  Close the keyboard device
 *------------------------------------------------------------------------
 */
devcall kbdclose(
	struct dentry *devptr /* Entry in device switch table	*/
)
{
	if (pid == getpid())
	{
		pid = -1;
		return 0;
	}

	return -1;
}