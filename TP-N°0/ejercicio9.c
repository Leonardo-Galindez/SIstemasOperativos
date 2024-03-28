#include <stdio.h>

void main() {
    char variable_char = 0x7F;//[-128,127]
    unsigned char variable_unsigned_char = 255;//[0,255] 65535 = 255 o 0xfffff

    printf("Valor de variable_char antes del incremento: %d \n", variable_char);
    printf("Valor de variable_unsigned_char antes del incremento: %u \n", variable_unsigned_char);
    
    variable_char++;
    variable_unsigned_char++;

    printf("Valor de variable_char después del incremento: %d\n", variable_char);
    printf("Valor de variable_unsigned_char después del incremento: %u\n", variable_unsigned_char);

}