#include <REGX52.H>
#include <intrins.h>
#include "glcd.h"
#include "car_bitmap.h"

// Game state variables
unsigned char car_x = 48;           // Car X position (32-80 for 4 lanes)
unsigned char car_y_page = 6;       // Car Y page (bottom of screen)

// Single obstacle instead of multiple - saves RAM
unsigned char obstacle_x = 32;
unsigned char obstacle_y_page = 0;
unsigned char obstacle_active = 1;

// Game controls (assuming buttons on P0)
sbit btn_left = P0^0;
sbit btn_right = P0^1;

// Button state variables for edge detection
unsigned char btn_left_prev = 1;
unsigned char btn_right_prev = 1;

void delay_ms(unsigned int ms) {
    unsigned int i;
    for(i = 0; i < ms; i++) {
        // Simple delay loop - removed nested loop to save stack
        _nop_();
        _nop_();
        _nop_();
        _nop_();
    }
}

void draw_road_lines() {
    unsigned char page;
    for (page = 0; page < 8; page++) {
        glcd_set_cursor(30, page);
        glcd_data(0xFF);  // left line
        glcd_set_cursor(96, page);
        glcd_data(0xFF);  // right line
    }
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

void draw_car() {
    glcd_draw_bitmap(car_x, car_y_page, 16, 2, car_bitmap);
}

void draw_obstacles() {
    if(obstacle_active) {
        glcd_draw_bitmap(obstacle_x, obstacle_y_page, 16, 1, obstacle_bitmap);
    }
}

void move_car_left() {
    if(car_x > 32) {  // Don't go past left road line
        clear_area(car_x, car_y_page, 16, 2);
        car_x -= 16;  // Move one lane
        draw_car();
    }
}

void move_car_right() {
    if(car_x < 80) {  // Don't go past right road line
        clear_area(car_x, car_y_page, 16, 2);
        car_x += 16;  // Move one lane
        draw_car();
    }
}

void move_obstacles() {
    if(obstacle_active) {
        // Clear old position
        clear_area(obstacle_x, obstacle_y_page, 16, 1);
        
        // Move down
        obstacle_y_page++;
        
        // If obstacle reaches bottom, reset to top
        if(obstacle_y_page > 6) {
            obstacle_y_page = 0;
            // Simple lane switching: alternate between 4 lanes
            if(obstacle_x == 32) obstacle_x = 48;
            else if(obstacle_x == 48) obstacle_x = 64;
            else if(obstacle_x == 64) obstacle_x = 80;
            else obstacle_x = 32;
        }
    }
}

unsigned char check_collision() {
    if(obstacle_active) {
        // Check collision: same lane and overlapping Y positions
        // Obstacle is now 1 page tall, car is 2 pages tall
        if(obstacle_x == car_x && 
           (obstacle_y_page >= car_y_page && obstacle_y_page <= car_y_page + 1)) {
            return 1; // Collision detected
        }
    }
    return 0; // No collision
}

void game_over() {
    unsigned char page;
    
    glcd_clear();
    // Simple "GAME OVER" display - draw horizontal bars
    for(page = 3; page < 5; page++) {
        glcd_set_cursor(20, page);
        glcd_data(0xFF);
        glcd_set_cursor(30, page);
        glcd_data(0xFF);
        glcd_set_cursor(40, page);
        glcd_data(0xFF);
        glcd_set_cursor(50, page);
        glcd_data(0xFF);
        glcd_set_cursor(60, page);
        glcd_data(0xFF);
        glcd_set_cursor(70, page);
        glcd_data(0xFF);
        glcd_set_cursor(80, page);
        glcd_data(0xFF);
        glcd_set_cursor(90, page);
        glcd_data(0xFF);
        glcd_set_cursor(100, page);
        glcd_data(0xFF);
    }
    while(1); // Stop game
}

void main() {
    unsigned int game_speed;
    
    game_speed = 0;
    
    glcd_init();
    glcd_clear();

    // Initial game state
    draw_road_lines();
    draw_car();
    draw_obstacles();

    while(1) {
        // Handle input with edge detection
        if(!btn_left && btn_left_prev) {  // Button pressed (falling edge)
            move_car_left();
        }
        btn_left_prev = btn_left;
        
        if(!btn_right && btn_right_prev) {  // Button pressed (falling edge)
            move_car_right();
        }
        btn_right_prev = btn_right;
        
        // Game logic - move obstacles periodically
        game_speed++;
        if(game_speed > 1000) {  // Increased from 500 to slow down the game
            game_speed = 0;
            
            // Check for collision BEFORE moving obstacles
            if(check_collision()) {
                game_over();
            }
            
            move_obstacles();
            draw_road_lines(); // Redraw road lines
            draw_obstacles();
            draw_car(); // Redraw car to ensure it's visible
        }
        
        delay_ms(10); // Small delay for game loop
    }
}
