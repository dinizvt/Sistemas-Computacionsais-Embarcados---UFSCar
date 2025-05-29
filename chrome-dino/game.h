#ifndef GAME_H
#define GAME_H

#define DINO_UP 1
#define DINO_DOWN 0
#define DINO    0x01
#define CACTUS  0x02
#define BIRD    0x03
#define LCD_LEN 16

#include "shared.h"

typedef struct {
    char top[LCD_LEN + 1];
    char bottom[LCD_LEN + 1];
} GameString;

extern unsigned char DINO_STATUS;

void initGame(GameString *g);
unsigned char updateGame(GameString *g, char newItem, char row);
unsigned char setDinoStatus(GameString *g, char status);
void intToStr(int num, char* str);

#endif