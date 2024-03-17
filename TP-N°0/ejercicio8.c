#include <stdio.h>

int main() {
    /*
        %zu
        indica que el siguiente argumento proporcionado es de tipo size_t
        y debe imprimirse como un valor sin signo. El tipo size_t se utiliza 
        comúnmente para representar tamaños de objetos en bytes, como los
        tamaños de matrices, estructuras, o el resultado del operador sizeof.
    */
    printf("El tamaño de char es: %zu bytes\n", sizeof(char));
    printf("El tamaño de short es: %zu bytes\n", sizeof(short));
    printf("El tamaño de int es: %zu bytes\n", sizeof(int));
    printf("El tamaño de long es: %zu bytes\n", sizeof(long));
    printf("El tamaño de long long es: %zu bytes\n", sizeof(long long));
    printf("El tamaño de float es: %zu bytes\n", sizeof(float));
    printf("El tamaño de double es: %zu bytes\n", sizeof(double));
    printf("El tamaño de long double es: %zu bytes\n", sizeof(long double));

    return 0;
}