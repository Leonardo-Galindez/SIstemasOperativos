
#define inportb(p)      inb(p)
#define outportb(p,v)   outb(p,v)

#define KEYBOARD_DATAREG 0x60  // :Data Register(Read\Write)
#define KEYBOARD_CMDREG 0x64   // :Command Register(Read\Write)


extern unsigned char kblayout [128];  // { ... } Fill your layout yourself 

//variable globales
sid32 semaforo = 0;

#define BUFFER_SIZE 10 // Tamaño del buffer

unsigned char buffer[BUFFER_SIZE];
int cabeza = 0;
int cola = 0;
int pos = 0;