#include "glcd.h"
#include "font_header.h"

// Optimized delay - removed unnecessary overhead
void delay_us(unsigned char us) {
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

void clear_area(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages) {
    unsigned char i;
    unsigned char j;
    for (j = 0; j < height_pages; j++) {
        for (i = 0; i < width; i++) {
            glcd_set_cursor(x + i, y_page + j);
            glcd_data(0x00);
        }
    }
}

// Simplified string function - optimized for memory usage
void glcd_string(unsigned char page, unsigned char x, char *str) {
	unsigned char i, j;
	
	for (i = 0; str[i] != 0; i++) {
		if (x > 122) break; // Leave space for character
		
		if (str[i] >= 32 && str[i] <= 90) { // Valid character range
			for (j = 0; j < 5; j++) {
				glcd_set_cursor(x + j, page);
				glcd_data(font[str[i] - 32][j]);
			}
			x += 6; // 5 pixels + 1 space
		}
	}
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

// Optimized clear function - reduced variable usage
void glcd_clear() {
	unsigned char page, col;
	for (page = 0; page < 8; page++) {
		for (col = 0; col < 128; col++) {
			glcd_set_cursor(col, page);
			glcd_data(0x00);
		}
	}
}

// Optimized bitmap drawing - reduced variable usage
void glcd_draw_bitmap(unsigned char x, unsigned char y_page, unsigned char width, unsigned char height_pages, unsigned char *bitmap) {
	unsigned char i, j;
	for (j = 0; j < height_pages; j++) {
		for (i = 0; i < width; i++) {
			glcd_set_cursor(x + i, y_page + j);
			glcd_data(bitmap[j * width + i]);
		}
	}
}


