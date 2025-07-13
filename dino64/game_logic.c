#include "game_logic.h"

// Game state variales definitions
Car car = {R_X_START, 6};

// Single obstacle instead of multiple - saves RAM
Obstacle obstacle = {0, 0}; // Obstacle position
unsigned char obstacle_active = 1;

// Left lane X positions initialized to starting X
unsigned char left_lane_x[8] = { R_X_START, R_X_START, R_X_START, R_X_START,
                                 R_X_START, R_X_START, R_X_START, R_X_START };

signed char curve_direction = 1;    // 1 = curving right, -1 = curving left
unsigned char curve_counter = 0;    // Counter for curve progression

// Game logic function implementations
void draw_road_lines() {
    unsigned char page;
    unsigned char right_x;
    unsigned char prev_left_x;
    unsigned char prev_right_x;
    unsigned char curve;
    
    for (page = N_PAGES-1; page > 0; page--) {
        prev_left_x = left_lane_x[page];  
        prev_right_x = prev_left_x + R_WIDTH;
        
        left_lane_x[page] = left_lane_x[page-1];
        right_x = left_lane_x[page] + R_WIDTH;
        
        clear_area(prev_left_x-CURB_W, page, CURB_W, 1);
        clear_area(prev_right_x, page, CURB_W, 1);
        glcd_draw_bitmap(left_lane_x[page]-CURB_W, page, CURB_W, 1, font[10]);  
        glcd_draw_bitmap(right_x, page, CURB_W, 1, font[10]);  
    }
    curve = curve_direction * 3;
    left_lane_x[0] += curve;
    if(left_lane_x[0] < 0) left_lane_x[0] = 0;
    if(left_lane_x[0]+R_WIDTH+CURB_W > 127) left_lane_x[0] = 127-CURB_W-R_WIDTH;
    // Do not draw first line (score)
}

void draw_car() {
    glcd_draw_bitmap(car.x, car.y, 14, 2, car_bitmap);
}

void draw_obstacles() {
    unsigned char obstacle_x;
    if(obstacle_active) {
        obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        glcd_draw_bitmap(obstacle_x, obstacle.y_page, OBSTACLE_W, 1, obstacle_bitmap);
    }
}

void move_car(unsigned char direction) {
    unsigned char new_x;
    unsigned char x_llim;
    unsigned char x_rlim;
    
    x_rlim = left_lane_x[car.y] + R_WIDTH - CAR_W;
    x_llim = left_lane_x[car.y];
    new_x = car.x + direction * CAR_W;

    if (new_x < x_llim) {
        if (car.x != x_llim) new_x = x_llim;  // Snap to left edge
    } else if (new_x > x_rlim) {
        if (car.x == x_rlim) new_x = x_rlim;
    }
    
    clear_area(car.x, car.y, CAR_W, 2);
    car.x = new_x;  // Update car position
    draw_car();
}

void move_obstacles() {
    if(obstacle_active) {
        // Clear old position - calculate where it was with curve
        unsigned char old_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        clear_area(old_x, obstacle.y_page, OBSTACLE_W, 1);
        
        // Move down
        obstacle.y_page++;
        
        // If obstacle reaches bottom, reset to top
        if(obstacle.y_page > 7) {
            obstacle.y_page = 1;
            // Cycle through lanes: 0->1->2->3->0
            obstacle.x_offset += OBSTACLE_W;  
            if(obstacle.x_offset+OBSTACLE_W > R_WIDTH) obstacle.x_offset = 0;  
        }
    }
}

void update_curve() {
    unsigned char curve;
    curve_counter++;
    // Change curve every 6 frames for faster, more acute transitions
    if(curve_counter >= 5) {
        curve = rand8() % 2;
        if (curve) {
            curve_direction = (rand8() % 2) ? 1 : -1;  // Randomly choose direction
            curve_counter = rand8() % 3;  
        } else {
            curve_direction = 0;
            curve_counter = 0;
        }
    }
}

unsigned char check_collision() {
    unsigned char obstacle_x;
    if(obstacle_active) {
        obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
        if (obstacle.y_page == car.y) {
            if (obstacle_x < car.x+CAR_W && obstacle_x+OBSTACLE_W > car.x) {
                return 1; // Collision detected
            }
        }
    }
    if (car.x < left_lane_x[car.y] || car.x + CAR_W > left_lane_x[car.y] + R_WIDTH) {
        return 1; // Collision with road edges
    }
    return 0; // No collision
}

void remove_obstacles_and_reset() {
    unsigned char obstacle_x = left_lane_x[obstacle.y_page] + obstacle.x_offset;
    clear_area(obstacle_x, obstacle.y_page, OBSTACLE_W, 1);
    clear_area(car.x, car.y, CAR_W, 2);
    glcd_string(6, left_lane_x[car.y] + 2, "CRASH!");
    delay_ms(2000); 
    clear_area(left_lane_x[car.y] + 2, 6, 35, 1); 
    car.x = left_lane_x[car.y] + CAR_W;
    draw_car();
    obstacle.y_page = 10;
    move_obstacles();
} 
    
void game_over(char *score_str) {
    glcd_clear();
    glcd_string(3, 30, "GAME OVER");
    glcd_string(4, 30, "SCORE: ");
    glcd_string(4, 65, score_str);
    while(1); // Stop game
}
