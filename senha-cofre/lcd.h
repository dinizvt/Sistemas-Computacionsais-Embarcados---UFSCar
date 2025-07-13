#ifndef LCD_H
#define LCD_H

#include <reg51.h> // Necessario para 'bit'

// Funcoes de controle do LCD
void returnHome(void);
void entryModeSet(bit id, bit s);
void displayOnOffControl(bit display, bit cursor, bit blinking);
void functionSet(void);
void clearDisplay(void);
void setDdRamAddress(char address);

// Funcoes de escrita no LCD
void sendChar(char c);
void sendString(char* str);

// Funcao auxiliar para manipulacao de bits
bit getBit(char c, char bitNumber);

#endif