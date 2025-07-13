#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H


#include <REGX52.H>
#include "glcd.h"
#include "car_bitmap.h"
#include "utils.h"

// Game constants
#define R_X_START 30
#define CAR_W 14
#define OBSTACLE_W 14
#define CURB_W 5
#define R_WIDTH (CAR_W * 5)
#define N_PAGES 8
#define RIGHT 1
#define LEFT -1

// Game state variables - declared as extern for use across files
extern unsigned char left_lane_x[N_PAGES];
extern signed char curve_direction;
extern unsigned char curve_counter;
extern unsigned char obstacle_active;

// Obstacle structure
typedef struct obstacle_t
{
    unsigned char x_offset;
    unsigned char y_page;
} Obstacle;
extern Obstacle obstacle;

typedef struct car_t
{
    unsigned char x;          // Car X position
    unsigned char y;     // Car Y page (bottom of screen)
} Car;
extern Car car;
// Game controls are defined in main.c

// Game logic function declarations
void draw_road_lines(void);
void draw_car(void);
void draw_obstacles(void);
void move_car(unsigned char direction);
void move_obstacles(void);
void update_curve(void);
unsigned char check_collision(void);
void game_over(char *score_str);
void remove_obstacles_and_reset(void);

#endif // GAME_LOGIC_H
