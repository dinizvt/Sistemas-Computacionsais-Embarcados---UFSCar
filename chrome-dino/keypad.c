#include "keypad.h"

sbit ROW1 = P0^3;
sbit ROW2 = P0^2;
sbit ROW3 = P0^1;
sbit ROW4 = P0^0;
sbit COL1 = P0^6;
sbit COL2 = P0^5;
sbit COL3 = P0^4;

const char keymap[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

char getKey(void) {
    char row;
    for(row = 0; row < 4; row++) {
        ROW1 = ROW2 = ROW3 = ROW4 = 1;

        switch(row) {
            case 0: ROW1 = 0; break;
            case 1: ROW2 = 0; break;
            case 2: ROW3 = 0; break;
            case 3: ROW4 = 0; break;
        }

        if(COL1 == 0 || COL2 == 0 || COL3 == 0) {
            delay_ms(10);

            if(COL1 == 0) { while(COL1 == 0); return keymap[row][0]; }
            if(COL2 == 0) { while(COL2 == 0); return keymap[row][1]; }
            if(COL3 == 0) { while(COL3 == 0); return keymap[row][2]; }
        }
    }
    return 0;
}