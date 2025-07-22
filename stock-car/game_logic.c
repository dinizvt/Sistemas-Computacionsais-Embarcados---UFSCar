#include "game_logic.h"

/* Initialize game state variables */
Car car = {R_X_START, CAR_START_PAGE};
Obstacle obstacle = {0, 0};
unsigned char obstacle_active = 1;

/* Initialize road with straight lane */
unsigned char left_lane_x[N_PAGES] = {
    R_X_START, R_X_START, R_X_START, R_X_START,
    R_X_START, R_X_START, R_X_START, R_X_START
};

signed char curve_direction = 1;    /* 1=right, -1=left */
unsigned char curve_counter = 0;

void draw_road_lines(void) {
    unsigned char page, right_x, prev_left_x, prev_right_x, curve;
    
    /* Update road positions from top to bottom */
    for (page = N_PAGES-1; page > 0; page--) {
        /* Save previous positions for cleanup */
        prev_left_x = left_lane_x[page];  
        prev_right_x = prev_left_x + R_WIDTH;
        
        /* Update lane position by taking value from row above */
        left_lane_x[page] = left_lane_x[page-1];
        right_x = left_lane_x[page] + R_WIDTH;
        
        /* Clear old road edges and draw new ones */
        clear_area(prev_left_x-CURB_W, page, CURB_W, 1);
        clear_area(prev_right_x, page, CURB_W, 1);
        glcd_draw_bitmap(left_lane_x[page]-CURB_W, page, CURB_W, 1, font[10]);  
        glcd_draw_bitmap(right_x, page, CURB_W, 1, font[10]);  
    }
    
    /* Apply curve to top row */
    curve = curve_direction * CURVE_INTENSITY;
    left_lane_x[0] += curve;
    
    /* Keep road within screen boundaries */
    if(left_lane_x[0] < 0) 
        left_lane_x[0] = 0;
    if(left_lane_x[0] + R_WIDTH + CURB_W > SCREEN_WIDTH - 1) 
        left_lane_x[0] = (SCREEN_WIDTH - 1) - CURB_W - R_WIDTH;
}


void draw_car(void) {
    glcd_draw_bitmap(car.x, car.y, CAR_W, CAR_H, car_bitmap);
}

void draw_obstacles(void) {
    if(obstacle_active) {
        unsigned char obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        glcd_draw_bitmap(obstacle_x, obstacle.y_page, OBSTACLE_W, OBSTACLE_H, obstacle_bitmap);
    }
}

void move_car(unsigned char direction) {
    unsigned char new_x;
    unsigned char x_llim = left_lane_x[car.y];        /* Left boundary */
    unsigned char x_rlim = x_llim + R_WIDTH - CAR_W;  /* Right boundary */
    
    new_x = car.x + direction * CAR_MOVE_STEP;

    /* Ensure car stays within road boundaries */
    if (new_x < x_llim) {
        if (car.x != x_llim) 
            new_x = x_llim;  /* Snap to left edge */
    } else if (new_x > x_rlim) {
        new_x = x_rlim;      /* Snap to right edge */
    }
    
    /* Update car position and redraw */
    clear_area(car.x, car.y, CAR_W, CAR_H);
    car.x = new_x;
    draw_car();
}

void move_obstacles(void) {
    if(obstacle_active) {
        /* Clear old position */
        unsigned char old_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        clear_area(old_x, obstacle.y_page, OBSTACLE_W, OBSTACLE_H);
        
        /* Move obstacle down one row */
        obstacle.y_page++;
        
        /* Reset obstacle when it reaches bottom */
        if(obstacle.y_page >= N_PAGES) {
            obstacle.y_page = START_PAGE;
            obstacle.x_offset = rand8() % (R_WIDTH - OBSTACLE_W);
        }
    }
}

void update_curve(void) {
    curve_counter++;
    
    /* Update curve direction periodically */
    if(curve_counter >= CURVE_MAX_COUNTER) {
        if (rand8() % 2) {
            /* Randomly choose a new curve direction */
            curve_direction = (rand8() % 2) ? RIGHT : LEFT;
            curve_counter = rand8() % 3;  
        } else {
            /* Occasionally go straight */
            curve_direction = 0;
            curve_counter = CURVE_MIN_COUNTER;
        }
    }
}

unsigned char check_collision(void) {
    /* Check collision with obstacle */
    if(obstacle_active && obstacle.y_page == car.y) {
        unsigned char obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        if (obstacle_x < car.x + CAR_W && obstacle_x + OBSTACLE_W > car.x) {
            return 1; /* Collision with obstacle */
        }
    }
    
    /* Check collision with road edges */
    if (car.x < left_lane_x[car.y] || 
        car.x + CAR_W > left_lane_x[car.y] + R_WIDTH) {
        return 1; /* Collision with road edge */
    }
    
    return 0; /* No collision */
}

void remove_obstacles_and_reset(void) {
    /* Clear obstacle and car from screen */
    unsigned char obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
    clear_area(obstacle_x, obstacle.y_page, OBSTACLE_W, OBSTACLE_H);
    clear_area(car.x, car.y, CAR_W, CAR_H);
    
    /* Display crash message */
    glcd_string(CRASH_MSG_PAGE, left_lane_x[car.y] + 2, "CRASH!");
    delay_ms(CRASH_DELAY);
    clear_area(left_lane_x[car.y] + 2, CRASH_MSG_PAGE, CRASH_MSG_LEN, 1);
    
    /* Reset car position */
    car.x = left_lane_x[car.y] + CAR_W;
    draw_car();
    
    /* Reset obstacle position */
    obstacle.y_page = START_PAGE;
    move_obstacles();
} 
    
void game_over(char *score_str) {
    /* Clear screen and display game over message with score */
    glcd_clear();
    glcd_string(GAME_OVER_PAGE, GAME_OVER_X, "GAME OVER");
    glcd_string(SCORE_PAGE, GAME_OVER_X, "SCORE: ");
    glcd_string(SCORE_PAGE, SCORE_X, score_str);
    
    /* Infinite loop to stop game execution */
    while(1);
}
