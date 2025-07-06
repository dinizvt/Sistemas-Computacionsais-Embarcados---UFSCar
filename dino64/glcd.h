#ifndef GLCD_H
#define GLCD_H

#include <REGX52.H>

// GLCD control pins (adjust to your Proteus circuit)
sbit RS = P2^0;
sbit RW = P2^1;
sbit EN = P2^2;
sbit CS1 = P2^3;
sbit CS2 = P2^4;
sbit RST = P2^5;

// Data port
#define DATA_PORT P1

void glcd_init();
void glcd_clear();
void glcd_command(unsigned char cmd);
void glcd_data(unsigned char d);
void glcd_set_cursor(unsigned char x, unsigned char y);
void glcd_display_bitmap(unsigned char *bitmap);
void glcd_draw_pixel(unsigned char x, unsigned char y, bit color);
void glcd_draw_bitmap(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages, unsigned char *bitmap);

#endif
