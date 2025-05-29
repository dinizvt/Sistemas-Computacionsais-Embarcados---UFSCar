#ifndef LCD_H
#define LCD_H

void sendChar(char c);
void sendString(char* str);
void lcdCommand(char command);
void initLCD(void);
void goToPosition(char row, char pos);
void clearDisplay(void);
bit getBit(char c, char bitNumber);

#endif