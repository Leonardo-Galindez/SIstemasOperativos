/* Compilar con: gcc -o ordenar ordenar.c -lpthread */

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int numeros[1000];
int final[1000];

void ordenar(void *n) {
	int i, j, desde, hasta, aux;

	desde = *((int *) n);
	hasta = desde + 500;
    
	/* ordenamos. metodo burbuja */
	for(i = desde; i < hasta-1; i++) {
		for(j = i+1; j < hasta; j++) {
			if (numeros[i] > numeros[j]) {
			         aux = numeros[i];
			         numeros[i] = numeros[j];
			         numeros[j] = aux;
			}
		}
	}
}

void fusionar_las_dos_mitades() {
	int i, j, pos;
       
	i = 0;		/* indice mitad izq */
	j = 500;	/* indice mitad der */
	pos = 0;	/* indice arreglo final */
    
	while(i < 500 || j < 1000) {

		if (i == 500) { 	/* si solo restan  nros de la der */
            		final[pos] = numeros[j];
			j++;

		} else if (j == 1000) { /* si solo restan nros de la izq */
            		final[pos] = numeros[i];
			i++;

		} else if (numeros[i] < numeros[j]) {
            		/* el numero de la primer mitad es menor */
            		final[pos] = numeros[i];
            		i++;

		} else {
            		/* el numero de la segunda mitad es menor */
			final[pos] = numeros[j];
			j++;
		}
        	pos++;
	}
}

void main() {
	int i, n, fd, t1, t2;
	pthread_t tid[2];
    
	/* cargamos los mil enteros */
	fd = open("desordenado.txt", O_RDONLY);
	for (i=0; i<1000; i++) {
		read(fd, &n, sizeof(int));
		numeros[i] = n;
	}
	close(fd);
    
	t1 = 0;
	t2 = 500;	/* segundo thread desde el indice 500 */
    
	pthread_create(&tid[0], NULL, (void*)ordenar, (void *)&t1);
	pthread_create(&tid[1], NULL, (void*)ordenar, (void *)&t2);
    
	/* esperamos a que los threads finalicen */
	for (int i = 0; i <= 1; i++) {
		pthread_join(tid[i], NULL);
	}
    
	fusionar_las_dos_mitades();
    
	/* mostramos el arreglo final */
	for(int i = 0; i < 1000; i++) {
		 printf("%i ", final[i]);
	}
}
