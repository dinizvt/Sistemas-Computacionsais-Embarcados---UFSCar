#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <REGX52.H>
#include "glcd.h"
#include "car_bitmap.h"
#include "utils.h"

/* Screen dimensions */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

/* Game layout constants */
#define N_PAGES 8                     /* Number of pages on screen */
#define R_X_START 30                  /* Initial road X position */
#define CAR_W 14                      /* Car width in pixels */
#define CAR_H 2                       /* Car height in pages */
#define OBSTACLE_W 14                 /* Obstacle width in pixels */
#define OBSTACLE_H 1                  /* Obstacle height in pages */
#define CURB_W 5                      /* Road edge width */
#define R_WIDTH (CAR_W * 5)           /* Road width in pixels */
#define CRASH_MSG_LEN 35              /* Length of crash message */

/* Game mechanics constants */
#define CURVE_MAX_COUNTER 5           /* Steps before curve change */
#define CURVE_INTENSITY 3             /* Road curve intensity */
#define CURVE_MIN_COUNTER 0           /* Min counter reset value */
#define CAR_MOVE_STEP CAR_W           /* Car movement distance */
#define CRASH_DELAY 2000              /* Delay after crash in ms */
#define START_PAGE 1                  /* Starting page for obstacles */
#define CAR_START_PAGE 6              /* Starting page for car */
#define GAME_OVER_X 30                /* Game over X position */
#define SCORE_X 65                    /* Score X position */
#define GAME_OVER_PAGE 3              /* Game over message page */
#define SCORE_PAGE 4                  /* Score display page */
#define CRASH_MSG_PAGE 6              /* Crash message page */

/* Movement directions */
#define RIGHT 1
#define LEFT -1

/* Game state variables - declared as extern for use across files */
extern unsigned char left_lane_x[N_PAGES];
extern signed char curve_direction;
extern unsigned char curve_counter;
extern unsigned char obstacle_active;

/* Obstacle structure */
typedef struct obstacle_t
{
    unsigned char x_offset;
    unsigned char y_page;
} Obstacle;
extern Obstacle obstacle;

/* Car structure */
typedef struct car_t
{
    unsigned char x;          /* Car X position */
    unsigned char y;          /* Car Y page (bottom of screen) */
} Car;
extern Car car;

/* Game logic function declarations */
void draw_road_lines(void);
void draw_car(void);
void draw_obstacles(void);
void move_car(unsigned char direction);
void move_obstacles(void);
void update_curve(void);
unsigned char check_collision(void);
void game_over(char *score_str);
void remove_obstacles_and_reset(void);

#endif /* GAME_LOGIC_H */
