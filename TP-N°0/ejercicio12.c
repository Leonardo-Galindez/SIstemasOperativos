#include <stdio.h>
#include <stdlib.h>	/* para las funciones system y exit */
#include <string.h>

int main() {
    char palabra[] = "lenguaje";
    char copiaPalabra[] = "lenguaje";
    int aciertos = 0;
    int intentos = 6;
	int c;
   
	/* Decirle al sistema que el modo input es RAW */
	system ("/bin/stty raw");
    /* Sirve para saltar a la linea de abajo en el principio*/
	while(1) {

        printf("\rPalabra: ");
        /*srtlen sirve para saber la longitud de una cadena de caracteres sin el 0*/
        for (int i = 0; i<=strlen(palabra); i++) {
            if (copiaPalabra[i] == c) {
                printf("%c ", c);
                aciertos ++;
                copiaPalabra[i] = '-';
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
            for(int i=0; i<=strlen(palabra) ;i++){
                if(copiaPalabra[i] == c){
                    acerto = 1;
                    break;
                }
            }

            if(!acerto){
                intentos--;
                if(intentos == 0){
                    printf("PERDISTE La palabra era: %s", palabra);
                    break;
                }
            }
        }
        if (aciertos == strlen(palabra)) {
            printf("\rGANASTE\r");
            break;
        }
	}
	system ("/bin/stty sane erase ^H");/*NO SE RECETEA*/
    printf("\nFin del juego\n");
    
}
