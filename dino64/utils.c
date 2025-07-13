#include "utils.h"
#include <intrins.h>

// LFSR state for rand8
static unsigned char lfsr_reg = 0xAE; // default seed (non-zero)

void srand8(unsigned char seed) {
    // Ensure non-zero seed
    lfsr_reg = (seed ? seed : 0xAE);
}

unsigned char rand8(void) {
    unsigned char lsb = lfsr_reg & 1;
    lfsr_reg >>= 1;
    if (lsb) {
        lfsr_reg ^= 0xB8; // polynomial x^8 + x^6 + x^5 + x^4 + 1
    }
    return lfsr_reg;
}

void delay_ms(unsigned int ms) {
    unsigned int i;
    unsigned int j;
    for(i = 0; i < ms; i++) {
        for (j = 0; j < 127; j++) {
            _nop_(); // No operation, just waste time
        }
    }
}

// Integer to string conversion function - optimized for 8051
// Converts unsigned int to string (max 5 digits: 65535)
void int_to_str(unsigned int num, char *str) {
    unsigned char i = 0;
    unsigned char j;
    char temp;
    
    // Handle zero case
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Extract digits in reverse order
    while (num > 0) {
        str[i] = (num % 10) + '0';  // Convert digit to ASCII
        num /= 10;
        i++;
    }
    
    // Null terminate
    str[i] = '\0';
    
    // Reverse the string
    for (j = 0; j < i / 2; j++) {
        temp = str[j];
        str[j] = str[i - 1 - j];
        str[i - 1 - j] = temp;
    }
}
