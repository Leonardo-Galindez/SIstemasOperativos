#include <stdio.h>
#include <stdlib.h>

int main() {
    char c;
    char palabra[] = "lenguaje";
    int intentos = 6;
    int letras_acertadas = 0;

    /* Decirle al sistema que el modo input es RAW */
    system ("/bin/stty raw");

    printf("Juego del Ahorcado\n");
    printf("Adivina la palabra secreta. Ingresa una letra (0 para salir).\n");

    while (1) {

        printf("\nIntentos restantes: %d\n", intentos);
		printf("\r c = %c  ingrese una letra (0 para salir): ", c);
        c = getchar();

        if (c == '0') {
            break;
        } else {
            //Verificamos en la palabra
            int acerto = 0;
            for (int i = 0; palabra[i] != '0'; i++) {
                if (palabra[i] == c) {
                    acerto = 1;
                    break;
                }
            }
            
            if (!acerto) {
                intentos--;
                if (intentos == 0) {
                    printf("\r¡Te quedaste sin intentos! La palabra era: %s\n", palabra);
                    break;
                }
            }
        }
    }

    system("/bin/stty sane erase ^H");
    printf("\nFin del juego\n");

    return 0;
}
