/* source: https://forum.osdev.org/viewtopic.php?t=10247 */

/* kbdhandler.c - kbdhandler */

#include <xinu.h>
#include <keyboard.h>

extern sid32 semaphore;
extern struct kbd_buffer buffer;

unsigned char get_scancode()
{
    unsigned char inputdata;
    inputdata = inportb(KEYBOARD_DATAREG);
    return inputdata;
}


/*------------------------------------------------------------------------
 *  kbdhandler  -  Handle an interrupt for the keyboard device
 *------------------------------------------------------------------------
 */
void kbdhandler(void)
{/
	char t[80];
	unsigned char scancode; 
	unsigned int shift_key = 0;
	int i = 10;

	scancode = get_scancode();
	//tecla_actual = scancode;
	//sprintf(t, "kbd: 0x%x     ", scancode);
	//print_text_on_vga(10, 300, t);
	
	if((buffer.fin+1) % BUFFER_SIZE != buffer.frente){
		buffer.array[buffer.fin] = scancode;
		//printf("caracter: %d",buffer.array[buffer.fin]);
		buffer.fin = (buffer.fin+1) % BUFFER_SIZE;
		signal(semaphore);
	}
		
	if(scancode == 0x2A) {
		shift_key = 1;//Shift key is pressed
	} else if(scancode & 0xAA) {
		shift_key = 0;//Shift Key is not pressed
	} else {          
		if (scancode & 0x80) {
			int shiftaltctrl = 1;//Put anything to see what special keys were pressed
		} else {  
		}     
	}
}
