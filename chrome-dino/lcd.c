#include "lcd.h"

#include <string.h>

sfr LCD_PORT = 0x90;
sbit DB7 = P1^7;
sbit DB6 = P1^6;
sbit DB5 = P1^5;
sbit DB4 = P1^4;
sbit RS  = P1^3;
sbit E   = P1^2;
sbit RW  = P1^1;

unsigned char DINOSAUR_MAP[8] = {
     0x04, // 00000100  (   *  )
     0x0E, // 00001110  (  *** )
     0x1F, // 00011111  ( *****)
     0x15, // 00010101  ( * * *)
     0x1F, // 00011111  ( *****)
     0x04, // 00000100  (   *  )
     0x0A, // 00001010  (  * * )
     0x11  // 00010001  ( *   *)
}; 

unsigned char CACTUS_MAP[8] = {
     0x00, // 00000000  (   *  )
     0x04, // 00000100  (  *** )
     0x05, // 00011111  ( *****)
     0x05, // 00010101  ( * * *)
     0x17, // 00011111  ( *****)
     0x1C, // 00000100  (   *  )
     0x04, // 00001010  (  * * )
     0x04  // 00010001  ( *   *)
}; 

unsigned char BIRD_MAP[8] = {
   0x00,
   0x04,
   0x09,
   0x17,
   0x04,
   0x02,
   0x00,
   0x00,
};



void lcdCommand(char cmd) {
   LCD_PORT = (LCD_PORT & 0x0F) | (cmd & 0xF0);
   RS = 0;
   RW = 0;
   E = 1;
   delay_ms(1);
   E = 0;
   delay_ms(2);
   
   LCD_PORT = (LCD_PORT & 0x0F) | (cmd << 4);
   E = 1;
   delay_ms(1);
   E = 0;
   delay_ms(5);
}

void lcdInit(void) {
   delay_ms(50);
   lcdCommand(0x02);
   lcdCommand(0x28);
   lcdCommand(0x0C);
   lcdCommand(0x06);
   lcdCommand(0x01);
   lcdCommand(0x80);
}

void clearDisplay(void) {
    lcdCommand(0x01);
}

void sendChar(char c) {
    LCD_PORT = (LCD_PORT & 0x0F) | (c & 0xF0);
    RS = 1; RW = 0; E=1;
    delay_ms(1);
    E = 0;
    delay_ms(2);
    
    LCD_PORT = (LCD_PORT & 0x0F) | (c << 4);
    E=1;
    delay_ms(1);
    E=0;
    delay_ms(2);
}

void sendString(char* str) {
    int index = 0;
    while(str[index] != 0) {
        sendChar(str[index++]);
    }
}

void createCustomChar(unsigned char charCode, unsigned char *pattern) {
    unsigned char i;
    charCode &= 0x07;
    lcdCommand(0x40|(charCode << 3)); // CGRAM address starts at 0x40
    for (i = 0; i < 8; i++) {
        sendChar(pattern[i]); // Send each row of the character
    }
}

void sendStringXY(char row, char pos, char *str) {
   if (row==0) lcdCommand ((pos & 0x0F) | 0x80);
   else if (row==1) lcdCommand((pos & 0x0F) | 0xC0);
   sendString(str);
}

void defineCustomChars() {
    createCustomChar(1, DINOSAUR_MAP); // DINO = 1
    createCustomChar(2, CACTUS_MAP); // CACTUS = 2
    createCustomChar(3, BIRD_MAP);
    delay_ms(2);
}