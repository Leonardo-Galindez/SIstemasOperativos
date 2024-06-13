#include <xinu.h>
#include <stdio.h>//libreria para las funcionesn printf ; getchar()
#include <string.h>//libreria para la funcion strlen 

//comando ahorcado para ejecutar
int ahorcado() {
    //tengo 2 arreglos , la copiaPalabra la voy modificando en la ejecucion y palabra no
    char palabra[] = "lenguaje";
    char copiaPalabra[] = "lenguaje";
    int aciertos = 0;
    int intentos = 6;
	int c;
	/* Decirle al sistema que el modo input es RAW (crudo) sin enter*/
    //&devtab[CONSOLE]. para obtener el puntero de la consola
    ttycontrol(&devtab[CONSOLE], TC_MODER,0, 0);
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
        if (aciertos == strlen(palabra)) {
            printf("\n \rGANASTE\r");
            break;
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
        
	}
    ttycontrol(&devtab[CONSOLE],TC_MODEC,0, 0);//vuelve a estado "sano" o cocinado
    printf("\nFin del juego\n");
    return 0;
}