#ifndef LCD_H
#define LCD_H

#include <reg51.h>
#include "shared.h"

void lcdCommand(char cmd);
void lcdInit(void);
void clearDisplay(void);
void setDdRamAddress(char address);
void sendChar(char c);
void sendString(char* str);
void sendStringXY(char row, char pos, char *str);
void createCustomChar(unsigned char charCode, unsigned char *pattern);
void defineCustomChars();

#endif