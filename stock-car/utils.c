#include "utils.h"
#include <intrins.h>

/* LFSR state for random number generation */
static unsigned char lfsr_reg = 0xAE;

void srand8(unsigned char seed) {
    lfsr_reg = (seed ? seed : 0xAE);
}

unsigned char rand8(void) {
    unsigned char lsb = lfsr_reg & 1;
    lfsr_reg >>= 1;
    if (lsb) {
        lfsr_reg ^= 0xB8; /* x^8 + x^6 + x^5 + x^4 + 1 */
    }
    return lfsr_reg;
}

void delay_ms(unsigned int ms) {
    unsigned int i;
    unsigned int j;
    for(i = 0; i < ms; i++) {
        for (j = 0; j < 127; j++) {
            _nop_();
        }
    }
}

/* Integer to string conversion (max 5 digits: 65535) */
void int_to_str(unsigned int num, char *str) {
    unsigned char i = 0;
    unsigned char j;
    char temp;
    
    /* Handle zero case */
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    /* Extract digits in reverse order */
    while (num > 0) {
        str[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }
    
    str[i] = '\0';
    
    /* Reverse the string */
    for (j = 0; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
}
