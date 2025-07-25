#ifndef GLCD_H
#define GLCD_H

#include <REGX52.H>

sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
sbit CS1 = P2^3;
sbit CS2 = P2^4;
sbit RST = P2^5;

#define DATA_PORT P1

extern unsigned char code font[][5];

void glcd_init();
void glcd_clear();
void glcd_command(unsigned char cmd);
void glcd_data(unsigned char d);
void glcd_string(unsigned char page, unsigned char x, char *str);
void glcd_set_cursor(unsigned char x, unsigned char y);
void glcd_draw_bitmap(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages, unsigned char *bitmap);
void clear_area(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages);

#endif
