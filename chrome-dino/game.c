#include "game.h"
#include <string.h>

unsigned char DINO_STATUS = DINO_DOWN;

void intToStr(int num, char* str) {
    int i = 0, j, rem;
    char temp[6];

    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    while (num > 0 && i < 5) {
        rem = num % 10;
        temp[i++] = rem + '0';
        num /= 10;
    }

    for (j = 0; j < i; j++) {
        str[j] = temp[i - j - 1];
    }
    str[i] = '\0';
}

void initGame(GameString *g) {
    unsigned char i;
    for (i=0; i<LCD_LEN; i++) {
      g->top[i] = ' ';
      g->bottom[i] = ' ';
    }
    g->bottom[0] = DINO;
}

unsigned char updateGame(GameString *g, char newItem, char row) {
    unsigned char i = 0;
    char *pos = (DINO_STATUS == DINO_DOWN) ? g->bottom : g->top;
    char obstacle = (DINO_STATUS == DINO_DOWN) ? CACTUS : BIRD;

    if (pos[1] == obstacle) return 1;

    do {
        g->top[i] = g->top[i + 1];
        g->bottom[i] = g->bottom[i + 1];
    } while(++i < LCD_LEN - 1);

    g->top[LCD_LEN-1] = ' ';
    g->bottom[LCD_LEN-1] = ' ';

    *pos = DINO;

    (row ? g->bottom : g->top)[LCD_LEN-1] = newItem;
    return 0;
}

unsigned char setDinoStatus(GameString *g, char status) {
    unsigned char obstacle = (status == DINO_DOWN) ? CACTUS : BIRD;
    unsigned char *position = (status == DINO_DOWN) ? g->bottom : g->top;

    if (position[0] == obstacle) return 1;
    
    DINO_STATUS = status;
    return 0;
}