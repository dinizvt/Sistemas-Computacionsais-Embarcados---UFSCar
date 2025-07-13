#ifndef UTILS_H
#define UTILS_H

// Utility function declarations
void delay_ms(unsigned int ms);
void int_to_str(unsigned int num, char *str);
// Random number generator (8-bit LFSR)
unsigned char rand8(void);
// Seed the pseudorandom generator
void srand8(unsigned char seed);

#endif // UTILS_H
