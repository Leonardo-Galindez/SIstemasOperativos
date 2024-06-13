/* bisiestos.c en XINU */

#include <xinu.h>

#define ASCII_ESC 27
long int calculo[2];
sid32 mutex;
int pid_pare = -1;
/* completar el codigo del programa bisiesto */

void mutex_init(void){
    mutex = semcreate(1);
}

void m_lock(void){
    wait(mutex);
    if(pid_pare == -1){
        pid_pare = getpid();
    }
}

void m_unlock(void){
    if(pid_pare == getpid()){
        pid_pare = -1;
        signal(mutex);
    }
}

int esBisiesto(int anio){
    int rta = 0;
    if(anio % 4 == 0){
        if(anio % 100 != 0){
            rta = 1;
        }else{
            if(anio % 400 == 0){
                rta = 1;
            }
        }   
    }
    return rta;
}

bisiesto(int base,int limite,int n){
	int i;
	/* base, limite y n vienen como argumentos */
    
	for (i=base; i<=limite; i++) {
		if (esBisiesto(i)){
            calculo[n] = i;	
			sleepms(30);
        }	
	}
}

muestra_bisiestos() {
	/* limpiar toda la pantalla */
	printf( "%c[2J", ASCII_ESC );
    //mutex
	while(1) {
		/* Nos posicionamos en la fila 5 columna 10 */
    		printf( "%c[5;10f", ASCII_ESC );
    		printf( "proceso a: %d \n", calculo[0]);

		/* Nos posicionamos en la fila 6 columna 10 */
    		printf( "%c[7;10f", ASCII_ESC );
    		printf( "proceso b: %d \n", calculo[1]);

		sleepms(10);
	}
}

/* completar el gestor de procesos bisiestos */
gestor_bisiestos(){   
    int pid_1,pid_2,pid_3;
    char tecla;
    mutex_init();
    pid_1 = create(bisiesto,1024,10,"p1",3,0,2000,0);
    pid_2 = create(bisiesto,1024,10,"p2",3,2001,4000,1);  
    resume(pid_1);
    resume(pid_2);
	while(1) {
		/* gestionar las pulsaciones del teclado, procesar, informar al usuario */
		sleepms(5);
        
        printf("Ingrese opcion\n");
        printf("suspender proceso 1--------1\n");
        printf("finalizar proceso 1--------3\n");
        printf("suspender proceso 2--------7\n");
        printf("finalizar proceso 2--------9\n");
        printf("finalizar------------------q\n");
        tecla = getchar();
        opciones(tecla,pid_1,pid_2);
        muestra_bisiestos();
	}
    
}

void opciones(char tecla,int pid_1,int pid_2){
    
    switch(tecla){
       case '1': 
            printf( "%c[9;10f", ASCII_ESC );
            printf( " 1er proceso suspendido\n");
            while(suspend(pid_1)== -1){
                
            }
            //si -1 esta suspendido
            if(1){
                resume(pid_1);
            }
        
       break;
       case '3':
            printf( "%c[9;10f", ASCII_ESC );
            printf( " 1er proceso finalizado\n");
            kill(pid_1);
       break;
        case '7':
            printf( "%c[9;10f", ASCII_ESC );
            printf( " 2do proceso suspendido\n");
            suspend(pid_2);
       break;

        case '9':
            printf( "%c[9;10f", ASCII_ESC );
            printf( " 2do proceso finalizado\n");
            kill(pid_2);
       break;
       case 'q':
            printf( "%c[9;10f", ASCII_ESC );
            printf( "todo finalizado\n");
            kill(pid_1);
            kill(pid_2);
       break;
    }
}