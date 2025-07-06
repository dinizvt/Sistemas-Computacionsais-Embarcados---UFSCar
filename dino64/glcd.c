#include "glcd.h"

void delay_us(unsigned int us) {
	while(us--);
}

void glcd_enable() {
	EN = 1;
	delay_us(2);
	EN = 0;
}

void glcd_select_chip(unsigned char x) {
	if (x < 64) {
		CS1 = 0;
		CS2 = 1;
	} else {
		CS1 = 1;
		CS2 = 0;
	}
}

void glcd_command(unsigned char cmd) {
	RW = 0;
	RS = 0;
	DATA_PORT = cmd;
	glcd_enable();
}

void glcd_data(unsigned char d) {
	RW = 0;
	RS = 1;
	DATA_PORT = d;
	glcd_enable();
}

void glcd_set_cursor(unsigned char x, unsigned char y) {
	glcd_select_chip(x);
	glcd_command(0xB8 | y); // Set page (0-7)
	glcd_command(0x40 | (x % 64)); // Set column (0-63)
}

void glcd_init() {
	RST = 0;
	delay_us(10);
	RST = 1;

	CS1 = 1;
	CS2 = 1;

	glcd_command(0x3F); // Display ON for left
	CS1 = 0;
	glcd_command(0x3F); // Display ON for right
	CS1 = 1;
}

void glcd_clear() {
	unsigned char page;
	unsigned char col;
	for (page = 0; page < 8; page++) {
		for (col = 0; col < 128; col++) {
			glcd_set_cursor(col, page);
			glcd_data(0x00);
		}
	}
}

void glcd_display_bitmap(unsigned char *bitmap) {
	unsigned char page;
	unsigned char col;
	for (page = 0; page < 8; page++) {
		for (col = 0; col < 128; col++) {
			glcd_set_cursor(col, page);
			glcd_data(bitmap[128 * page + col]);
		}
	}
}

void glcd_draw_bitmap(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages, unsigned char *bitmap) {
	unsigned char i;
	unsigned char j;
	for (j = 0; j < height_pages; j++) {
		for (i = 0; i < width; i++) {
			glcd_set_cursor(x + i, y_page + j);
			glcd_data(bitmap[j * width + i]);
		}
	}
}


