#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */
#include <string.h>

int main() {
    //tengo 2 arreglos , la copiaPalabra la voy modificando en la ejecucion y palabra no
    char palabra[] = "lenguaje";
    char copiaPalabra[] = "lenguaje";
    int aciertos = 0;
    int intentos = 6;
	int c;
   
	/* Decirle al sistema que el modo input es RAW (crudo) sin enter*/
	system ("/bin/stty raw");
    /* Sirve para saltar a la linea de abajo en el principio*/
	while(1) {
        /*Mostramos la palabra oculta*/
        printf("\rPalabra: ");
        /*srtlen sirve para saber la longitud de una cadena de caracteres sin el 0*/
        for (int i = 0; i<strlen(palabra); i++) {
            //este for va mostrardo si acertamos una
            if (copiaPalabra[i] == c) {
                printf("%c ", c);
                aciertos ++;
                copiaPalabra[i] = '-';//marcamos los aciertos
            } else {
                printf("_ ");
            }
        }

        printf("      Intentos restantes: %d\n", intentos);
		printf("\r c = %c  ingrese una letra (0 para salir): ", c);
		c = getchar();
        

		if (c == '0'){
            break;
        }else{
            int acerto = 0;
            //verificamos si esta la letra en la cadena
            for(int i=0; i<strlen(palabra) ;i++){
                if(copiaPalabra[i] == c){
                    acerto = 1;
                    break;
                }
            }
            //si no esta descontamos los intentos
            if(!acerto){
                intentos--;
                if(intentos == 0){
                    printf("\rPERDISTE La palabra era: %s", palabra);
                    break;
                }
            }
        }
        if (aciertos == strlen(palabra)-1) {
            printf("\rGANASTE\r");
            break;
        }
	}
	system ("/bin/stty sane erase ^H");//vuelve a estado "sano"
    printf("\nFin del juego\n");
    
}
