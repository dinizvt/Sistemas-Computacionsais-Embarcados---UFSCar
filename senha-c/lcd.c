#include <reg51.h>
#include "lcd.h"
#include "shared.h"

sfr LCD_PORT = 0x90;
sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS  = P1^3;
sbit E   = P1^2;
sbit RW = P1^1;

void lcdCommand(char command) {
	LCD_PORT = (LCD_PORT & 0x0F) | (command & 0xF0);
	RS = 0;
	RW = 0;
	E = 1;
	delay_ms(1);
	E=0;
	delay_ms(2);
	
	LCD_PORT = (LCD_PORT & 0x0F) | (command << 4);
	E=1;
	delay_ms(1);
	E=0;
	delay_ms(5);
}

void initLCD(void) {
	delay_ms(20);
	lcdCommand(0x02);
	lcdCommand(0x28);
	lcdCommand(0x0C);
	lcdCommand(0x06);
	lcdCommand(0x01);
	lcdCommand(0x80);
}

void clearDisplay() {
	lcdCommand(0x01);
}

void goToPosition(char row, char pos) {
	if (row==0) lcdCommand((pos&0x0F)|0x80);
	else lcdCommand((pos&0x0F)|0xC0);
}

void sendChar(char c) {
    DB7 = getBit(c, 7); DB6 = getBit(c, 6); DB5 = getBit(c, 5); DB4 = getBit(c, 4); RS = 1; E = 1; E = 0;
    DB7 = getBit(c, 3); DB6 = getBit(c, 2); DB5 = getBit(c, 1); DB4 = getBit(c, 0); E = 1; E = 0;
    delay_ms(1);
}

void sendString(char* str) {
    int index = 0;
    while(str[index] != 0) {
        sendChar(str[index++]);
    }
}

bit getBit(char c, char bitNumber) {
    return (c >> bitNumber) & 1;
}