#include <REGX52.H>
#include "glcd.h"
#include "bitmaps.h"
#include "game_logic.h"
#include "utils.h"

/* Game controls (P0 pins) */
sbit btn_left = P0^0;
sbit btn_right = P0^1;

/* Button state for edge detection */
unsigned char btn_left_prev = 1;
unsigned char btn_right_prev = 1;
unsigned int seed_time;
unsigned char lives = 3;

/* Function to display score and lives */
void display_game_stats(unsigned int score, unsigned char lives) {
    char score_str[6];
    char lives_str[2];
    
    int_to_str(score, score_str);
    int_to_str(lives, lives_str);
    
    glcd_string(0, 0, "SCORE: ");
    glcd_string(0, 35, score_str);
    glcd_string(0, 82, "LIVES: ");
    glcd_string(0, 122, lives_str);
}

void main() {
    unsigned int game_speed;
    unsigned int score = 0;

    game_speed = 0;
    
    glcd_init();
    glcd_clear();
    
    /* Start screen and random seed based on reaction time */
    glcd_string(3, 15, "PRESS ANY BUTTON");
    seed_time = 0;
    while (btn_left && btn_right) {
        delay_ms(1);
        seed_time++;
    }
    delay_ms(100);
    srand8((unsigned char)seed_time);
    glcd_clear();
    
    /* Initial game setup */
    draw_road_lines();
    draw_car();
    draw_obstacles();

    while(1) {
        /* Button input with edge detection */
        if(!btn_left && btn_left_prev) {
            move_car(LEFT);
        }
        btn_left_prev = btn_left;
        
        if(!btn_right && btn_right_prev) {
            move_car(RIGHT);
        }
        btn_right_prev = btn_right;
        /* Game update at fixed intervals */
        game_speed++;
        if(game_speed > 250) {
            score += 10;
            game_speed = 0;
            
            move_obstacles();
            update_curve();
            
            draw_road_lines();
            draw_car();
            draw_obstacles();
            
            if(check_collision()) {
                lives--;
                if (lives == 0) {
                    char score_str[6];
                    int_to_str(score, score_str);
                    game_over(score_str);
                }
                else remove_obstacles_and_reset();
            }
            display_game_stats(score, lives);
        }
        delay_ms(1);
    }
}
