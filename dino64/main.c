#include <REGX52.H>
#include "glcd.h"
#include "car_bitmap.h"
#include "game_logic.h"
#include "utils.h"

// Game controls (buttons on P0 pins)
// These must be defined once in main.c
sbit btn_left = P0^0;
sbit btn_right = P0^1;
// Button state variables for edge detection
unsigned char btn_left_prev = 1;
unsigned char btn_right_prev = 1;
unsigned int seed_time;
unsigned char lives = 3;

void main() {
    unsigned int game_speed;
    unsigned int score = 0;

    game_speed = 0;
    
    glcd_init();
    glcd_clear();
    
    // Show start screen and seed random generator based on reaction time
    glcd_string(3, 15, "PRESS ANY BUTTON");
    seed_time = 0;
    // Wait until either button is pressed
    while (btn_left && btn_right) {
        delay_ms(1);
        seed_time++;
    }
    delay_ms(100); // Debounce delay
    srand8((unsigned char)seed_time);
    glcd_clear();
    
    // Initial game state
    draw_road_lines();
    draw_car();
    draw_obstacles();

    while(1) {
        // Handle input with edge detection
        if(!btn_left && btn_left_prev) {  // Button pressed (falling edge)
            move_car(LEFT);
        }
        btn_left_prev = btn_left;
        
        if(!btn_right && btn_right_prev) {  // Button pressed (falling edge)
            move_car(RIGHT);
        }
        btn_right_prev = btn_right;
        
        // Game logic - move obstacles periodically
        game_speed++;
        if(game_speed > 300) {  // Increased from 500 to slow down the game
            char score_str[6];  // Buffer for score string (max 5 digits + null)
            char lives_str[2]; 
            score+=10;
            game_speed = 0;
              // Check for collision BEFORE moving obstacles
            
            
            move_obstacles();
            update_curve();      // Update curve progression
            
            // Clear and redraw everything for smooth curve animation
            draw_road_lines();   // Redraw curved road lines
            draw_car();          // Draw car first
            draw_obstacles();    // Draw obstacles LAST to ensure they're visible
            
            if(check_collision()) {
                lives--;  // Decrease lives on collision
                if (lives == 0) game_over(score_str);
                else remove_obstacles_and_reset();
            }
            // Display score using our int_to_str function
            int_to_str(score, score_str);
            int_to_str(lives, lives_str);
            glcd_string(0, 0, "SCORE: ");
            glcd_string(0, 35, score_str);
            glcd_string(0, 82, "LIVES: ");
            glcd_string(0, 122, lives_str);
        }
        delay_ms(1); // Small delay for game loop
    }
}
