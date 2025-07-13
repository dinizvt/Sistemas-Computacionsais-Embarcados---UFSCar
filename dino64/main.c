#include <REGX52.H>
#include <intrins.h>
#include "glcd.h"
#include "car_bitmap.h"

#define R_LANE_W 16
#define R_N_LANES 4
#define R_X_START 30
#define R_LIMIT_W 2
// Game state variables
unsigned char car_x = R_LANE_W+R_X_START+R_LIMIT_W;           // Car X position (32-80 for 4 lanes)
unsigned char car_y_page = 6;       // Car Y page (bottom of screen)

// Single obstacle instead of multiple - saves RAM
unsigned char obstacle_x = R_X_START+R_LIMIT_W;
unsigned char obstacle_y_page = 0;
unsigned char obstacle_active = 1;
unsigned char obstacle_lane = 0;    // 0=left, 1=center-left, 2=center-right, 3=right
unsigned char left_lane_x[8] = {R_X_START, R_X_START, R_X_START, R_X_START, R_X_START, R_X_START, R_X_START, R_X_START} ;

// Curve variables
signed char curve_offset = 0;       // Current curve offset (-30 to +30) - more acute
signed char curve_direction = 1;    // 1 = curving right, -1 = curving left
unsigned char curve_counter = 0;    // Counter for curve progression

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

void draw_road_lines() {
    unsigned char page;
    unsigned char left_x, right_x;
    unsigned char prev_left_x;
    unsigned char prev_right_x;
    
    for (page = 0; page < 8; page++) {
        // Calculate curve position for this page with more acute curve
        // Multiply by 2 for more pronounced curve effect
        signed char page_curve = (curve_offset * (signed char)(7 - page) * 2) / 7;
        prev_left_x = left_lane_x[page];  
        prev_right_x = left_lane_x[page] + R_LANE_W*R_N_LANES + R_LIMIT_W;
        
        left_lane_x[page] = R_X_START + page_curve;
        right_x = left_lane_x[page] + R_LANE_W*R_N_LANES + R_LIMIT_W;
        
        // Ensure boundaries (keep road markers on screen)
        if(left_x < 5) left_x = 5;
        if(left_x > 60) left_x = 60;
        if(right_x < 60) right_x = 60;
        if(right_x > 122) right_x = 122;
        
        clear_area(prev_left_x, page, 2, 1);
        clear_area(prev_right_x, page, 2, 1);
        
        // Draw better visible "*" pattern for road markers
        // Use multiple bytes to create more visible stars
        glcd_set_cursor(left_lane_x[page], page);
        glcd_data(0x3C);
        glcd_set_cursor(left_lane_x[page] + 1, page);
        glcd_data(0x3C);  
        
        glcd_set_cursor(right_x, page);
        glcd_data(0x3C);
        glcd_set_cursor(right_x + 1, page);
        glcd_data(0x3C);
    }
}

void draw_car() {
    glcd_draw_bitmap(car_x, car_y_page, 14, 2, car_bitmap);
}

void draw_obstacles() {
    if(obstacle_active) {
        // Now that obstacles work, add back curve following
        signed char page_curve = (curve_offset * (signed char)(7 - obstacle_y_page)) / 4;  // Gentler curve
        
        // Calculate base lane position with curve offset
        obstacle_x = R_X_START + (obstacle_lane * R_LANE_W) + page_curve + R_LIMIT_W;
        
        // Ensure obstacle stays within screen bounds
        if(obstacle_x < 20) obstacle_x = 20;
        if(obstacle_x > 90) obstacle_x = 90;
        
        glcd_draw_bitmap(obstacle_x, obstacle_y_page, 16, 1, obstacle_bitmap);
    }
}

void move_car_left() {
    if(car_x > R_X_START+R_LIMIT_W) {  // Don't go past left road line
        clear_area(car_x, car_y_page, R_LANE_W, 2);
        car_x -= R_LANE_W;  // Move one lane
        draw_car();
    }
}

void move_car_right() {
    if(car_x < 80) {  // Don't go past right road line
        clear_area(car_x, car_y_page, R_LANE_W, 2);
        car_x += R_LANE_W;  // Move one lane
        draw_car();
    }
}

void move_obstacles() {
    if(obstacle_active) {
        // Clear old position - calculate where it was with curve
        signed char old_page_curve = (curve_offset * (signed char)(7 - obstacle_y_page)) / 4;
        unsigned char old_x = R_X_START + R_LIMIT_W + (obstacle_lane * R_LANE_W) + old_page_curve;
        if(old_x < 20) old_x = 20;
        if(old_x > 90) old_x = 90;
        clear_area(old_x, obstacle_y_page, R_LANE_W, 1);
        
        // Move down
        obstacle_y_page++;
        
        // If obstacle reaches bottom, reset to top
        if(obstacle_y_page > 6) {
            obstacle_y_page = 0;
            // Cycle through lanes: 0->1->2->3->0
            obstacle_lane++;
            if(obstacle_lane > 3) obstacle_lane = 0;
        }
    }
}

void update_curve() {
    curve_counter++;
    
    // Change curve every 6 frames for faster, more acute transitions
    if(curve_counter >= 6) {
        curve_counter = 0;
        curve_offset += curve_direction * 2;  // Faster curve progression
        
        // Reverse direction when reaching curve limits (more extreme)
        if(curve_offset >= 25) {
            curve_direction = -1;
        } else if(curve_offset <= -25) {
            curve_direction = 1;
        }
    }
}

unsigned char check_collision() {
    unsigned char car_lane;
    
    if(obstacle_active) {
        // Simplified collision detection using lane-based approach
        car_lane = (car_x - R_X_START+R_LIMIT_W) / R_LANE_W;  // Convert car position to lane number
        
        // Check if obstacle and car are in same lane and overlapping vertically
        if(obstacle_lane == car_lane && 
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
            update_curve();      // Update curve progression
            
            // Clear and redraw everything for smooth curve animation
            //clear_area(0, 0, 128, 8);  // Clear entire screen area
            draw_road_lines();   // Redraw curved road lines
            draw_car();          // Draw car first
            draw_obstacles();    // Draw obstacles LAST to ensure they're visible
        }
        
        delay_ms(10); // Small delay for game loop
    }
}
