#include "shared.h"

// Implementacao da funcao de atraso
void delay_ms(unsigned int ms) {
    unsigned int i, j;
    // O valor 127 eh uma constante de ajuste para o clock do microcontrolador
    // Pode precisar ser ajustado dependendo da frequencia do cristal do 8051
    for(i = 0; i < ms; i++)
        for(j = 0; j < 127; j++);
}