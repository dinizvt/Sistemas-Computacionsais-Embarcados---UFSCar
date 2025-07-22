#include "address_map_arm.h"
#include "dino_sprite.h"
#include "cactus_sprite.h"
#include "bird_sprite.h"
#include <stdlib.h>

// Function declarations
void video_text(int x, int y, char *text);
void video_box(int x1, int y1, int x2, int y2, short color);
void clear_screen(void);
void clear_text_screen(void);  // Add this line
void draw_dino(int x, int y, int frame);
void draw_obstacle(int x, int y, int type);
void draw_bird(int x, int y);
void draw_ground(void);
void draw_score(int score);
int  resample_rgb(int bits, int color);
int  get_data_bits(int mode);
int  check_collision(int dino_x, int dino_y, int obs_x, int obs_y, int obs_type);
void delay(int count);
int  random_range(int min, int max);

// Game state management
void init_game_state(void);
void reset_game_state(void);
void handle_jump_logic(volatile int *key_ptr);
void handle_obstacle_movement(void);
void render_game(void);
void show_start_screen(void);
void show_game_over_screen(int final_score);
void int_to_string(int num, char *str, int start_pos);

// Game configuration constants
#define STANDARD_X 320
#define STANDARD_Y 240
#define GROUND_Y 200
#define JUMP_HEIGHT 50
#define JUMP_FRAMES 10
#define GAME_SPEED 100000
#define DINO_START_X 30
#define MIN_OBSTACLE_SPAWN_FRAMES 30  // Minimum frames between spawns
#define MAX_OBSTACLES_SPAWN_FRAMES 60 // Maximum frames between spawns
#define SCORE_INCREMENT 10
#define FRAME_SCORE_INCREMENT 1
#define SCORE_FRAME_INTERVAL 2
#define SPEED_INCREASE_FACTOR 20
#define OBSTACLE_STEP 4

// Obstacle types
#define OBSTACLE_TYPE_CACTUS 0
#define OBSTACLE_TYPE_BIRD 1

// Jump states
#define JUMP_NONE 0
#define JUMP_UP 1
#define JUMP_HOLD 2
#define JUMP_DOWN 3

#define JUMP_HOLD_FRAMES 5  // Number of frames to hold at the top

// Color constraints
int highlights_color = 0xFFFFFF; // White for highlights_color
int background_color = 0x000000; // Black for background_color
int inverted_colors = 0;

// Display configuration
int screen_x, screen_y;
int res_offset, col_offset, db;
int dino_leg = 0;

// Game state variables
typedef struct {
    int x, y;
    int jump_state;
    int jump_counter;
} Dino;

typedef struct {
    int x, y;
    int type;  // OBSTACLE_TYPE_CACTUS or OBSTACLE_TYPE_BIRD
    int active;
} Obstacle;

typedef struct {
    int score;
    int game_over;
    int frame_counter;
    int current_speed;
} GameState;

Dino dino;
// Allow up to 3 obstacles on screen at once
#define MAX_OBSTACLES 3
Obstacle obstacles[MAX_OBSTACLES];

GameState game;

// Timer to control time between obstacle spawns
int obstacle_spawn_timer = 0;

int main(void) {
    volatile int *key_ptr = (int *)KEY_BASE;
    
    init_game_state();
    
    while (1) {
        reset_game_state();
        show_start_screen();
        
        // Wait for key release then press to start
        while ((*key_ptr & 0x2) != 0);
        delay(1000);
        while ((*key_ptr & 0x2) == 0);
        
        clear_screen();
        clear_text_screen();  
        draw_ground();
        // Main game loop
        while (!game.game_over) {
            handle_jump_logic(key_ptr);
            handle_obstacle_movement();
            int i;
            for (i = 0; i < MAX_OBSTACLES; i++) {
                if (obstacles[i].active && check_collision(dino.x, dino.y, obstacles[i].x, obstacles[i].y, obstacles[i].type)) {
                    game.game_over = 1;
                }
            }
            // Increment score every N frames
            if (game.frame_counter % SCORE_FRAME_INTERVAL == 0) {
                game.score += FRAME_SCORE_INCREMENT;
                if (game.score % SPEED_INCREASE_FACTOR == 0 && game.current_speed > 10000) {
                    game.current_speed -= 10000;
                }
                if (game.score % 200 == 0) {
                    int aux;
                    aux = background_color;
                    background_color = highlights_color;
                    highlights_color = aux;
                    inverted_colors = 100;
                }
                if (inverted_colors == 1) {
                    int aux;
                    aux = background_color;
                    background_color = highlights_color;
                    highlights_color = aux;
                    inverted_colors = 0;
                }
                else if (inverted_colors > 0) {
                    inverted_colors--;
                }   
            }
            render_game();
            delay(game.current_speed);
            game.frame_counter++;
        }
        
        show_game_over_screen(game.score);
        
        // Wait for restart
        while ((*key_ptr & 0x2) != 0);
        while ((*key_ptr & 0x2) == 0);
    }
    
    return 0;
}

void clear_screen(void) {
    video_box(0, 0, STANDARD_X, STANDARD_Y, resample_rgb(db, background_color));
}

void draw_dino(int x, int y, int frame) {
    short dino_color = resample_rgb(db, highlights_color);
    int row, col;
    int pixel;
    if (frame % 5 == 0) {
        dino_leg = !dino_leg;
    }
    for (row = 0; row < DINO_SPRITE_HEIGHT; row++) {
        for (col = 0; col < DINO_SPRITE_WIDTH; col++) {
            pixel = dino_get_pixel(row, col, dino_leg);
            if (pixel) {
                video_box(x + col, y + row, x + col, y + row, dino_color);
            }
        }
    }
}

void draw_obstacle(int x, int y, int type) {
    if (type == OBSTACLE_TYPE_CACTUS) {
        if (x >= 0 && x < STANDARD_X) {
            short obstacle_color = resample_rgb(db, highlights_color);
            int row, col;
            for (row = 0; row < CACTUS_SPRITE_HEIGHT; row++) {
                for (col = 0; col < CACTUS_SPRITE_WIDTH; col++) {
                    if (cactus_get_pixel(row, col)) {
                        video_box(x + col, y + row, x + col, y + row, obstacle_color);
                    }
                }
            }
        }
    } else if (type == OBSTACLE_TYPE_BIRD) {
        draw_bird(x, y);
    }
}

void draw_bird(int x, int y) {
    if (x >= 0 && x < STANDARD_X) {
        short bird_color = resample_rgb(db, highlights_color);
        int row, col;
        for (row = 0; row < BIRD_FRAME_HEIGHT; row++) {
            for (col = 0; col < BIRD_FRAME_WIDTH; col++) {
                uint32_t pixel = bird_data[0][row * BIRD_FRAME_WIDTH + col];
                if (pixel != 0x00000000) {  // If not transparent
                    video_box(x + col, y + row, x + col, y + row, bird_color);
                }
            }
        }
    }
}

void draw_ground(void) {
    short ground_color = resample_rgb(db, highlights_color);
    video_box(0, GROUND_Y, STANDARD_X, GROUND_Y + 2, ground_color);
}

void draw_score(int score) {
    char score_display[20] = "Score: ";
    int_to_string(score, score_display, 7);
    video_text(1, 1, score_display);
}

int check_collision(int dino_x, int dino_y, int obs_x, int obs_y, int obs_type) {
    int obs_width, obs_height;
    
    if (obs_type == OBSTACLE_TYPE_CACTUS) {
        obs_width = CACTUS_SPRITE_WIDTH;
        obs_height = CACTUS_SPRITE_HEIGHT;
    } else if (obs_type == OBSTACLE_TYPE_BIRD) {
        obs_width = BIRD_FRAME_WIDTH;
        obs_height = BIRD_FRAME_HEIGHT;
    } else {
        return 0;  // Unknown obstacle type
    }
    
    return (dino_x < obs_x + obs_width + 3 &&
            dino_x + DINO_SPRITE_WIDTH > obs_x &&
            dino_y < obs_y + obs_height &&
            dino_y + DINO_SPRITE_HEIGHT > obs_y);
}

// Helper function implementations
void init_game_state(void) {
    volatile int *video_resolution = (int *)(PIXEL_BUF_CTRL_BASE + 0x8);
    screen_x = *video_resolution & 0xFFFF;
    screen_y = (*video_resolution >> 16) & 0xFFFF;

    volatile int *rgb_status = (int *)(RGB_RESAMPLER_BASE);
    db = get_data_bits(*rgb_status & 0x3F);

    res_offset = (screen_x == 160) ? 1 : 0;
    col_offset = (db == 8) ? 1 : 0;
    srand(42);
}

void reset_game_state(void) {
    dino.x = DINO_START_X;
    dino.y = GROUND_Y - DINO_SPRITE_HEIGHT;
    dino.jump_state = JUMP_NONE;
    dino.jump_counter = 0;

    // Initialize obstacles: only the first is active at start
    int i;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = 0;
    }
    obstacles[0].x = STANDARD_X;
    // Only cactus at the start
    obstacles[0].type = OBSTACLE_TYPE_CACTUS;
    obstacles[0].y = GROUND_Y - CACTUS_SPRITE_HEIGHT;
    obstacles[0].active = 1;

    game.score = 0;
    game.game_over = 0;
    game.frame_counter = 0;
    game.current_speed = GAME_SPEED;
    obstacle_spawn_timer = 0;
}

void handle_jump_logic(volatile int *key_ptr) {
    static int jump_hold_counter = 0;
    if ((*key_ptr & 0x1) != 0 && dino.jump_state == JUMP_NONE) {
        dino.jump_state = JUMP_UP;
        dino.jump_counter = 0;
        jump_hold_counter = 0;
    }

    if (dino.jump_state == JUMP_UP) {
        dino.jump_counter++;
        dino.y = (GROUND_Y - DINO_SPRITE_HEIGHT) - (JUMP_HEIGHT * dino.jump_counter / JUMP_FRAMES);
        if (dino.jump_counter >= JUMP_FRAMES) {
            dino.jump_state = JUMP_HOLD;
            jump_hold_counter = 0;
        }
    } else if (dino.jump_state == JUMP_HOLD) {
        // Hold at the top for a few frames
        dino.y = (GROUND_Y - DINO_SPRITE_HEIGHT) - JUMP_HEIGHT;
        jump_hold_counter++;
        if (jump_hold_counter >= JUMP_HOLD_FRAMES) {
            dino.jump_state = JUMP_DOWN;
        }
    } else if (dino.jump_state == JUMP_DOWN) {
        dino.jump_counter--;
        dino.y = (GROUND_Y - DINO_SPRITE_HEIGHT) - (JUMP_HEIGHT * dino.jump_counter / JUMP_FRAMES);
        if (dino.jump_counter <= 0) {
            dino.jump_state = JUMP_NONE;
            dino.y = GROUND_Y - DINO_SPRITE_HEIGHT;
        }
    }
}

void handle_obstacle_movement(void) {
    int i;
    int obstacle_rand;

    // Move all active obstacles
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (!obstacles[i].active) continue;
        obstacles[i].x -= OBSTACLE_STEP;
        int obstacle_width = (obstacles[i].type == OBSTACLE_TYPE_CACTUS) ? CACTUS_SPRITE_WIDTH : BIRD_FRAME_WIDTH;
        if (obstacles[i].x < -obstacle_width) {
            obstacles[i].active = 0; // Mark as inactive, ready for respawn
        }
    }

    // Handle obstacle spawn timing
    if (obstacle_spawn_timer > 0) {
        obstacle_spawn_timer--;
    } else {
        for (i = 0; i < MAX_OBSTACLES; i++) {
            if (!obstacles[i].active) {
                obstacles[i].x = STANDARD_X;
                // Only spawn birds if score >= 50
                if (game.score >= 50) {
                    obstacle_rand = random_range(0, 2);
                    obstacles[i].type = obstacle_rand == 0 ? OBSTACLE_TYPE_BIRD : OBSTACLE_TYPE_CACTUS;
                } else {
                    obstacles[i].type = OBSTACLE_TYPE_CACTUS;
                }
                if (obstacles[i].type == OBSTACLE_TYPE_CACTUS) {
                    obstacles[i].y = GROUND_Y - CACTUS_SPRITE_HEIGHT;
                } else if (obstacles[i].type == OBSTACLE_TYPE_BIRD) {
                    obstacles[i].y = GROUND_Y - DINO_SPRITE_HEIGHT - BIRD_FRAME_HEIGHT - 10;
                }
                obstacles[i].active = 1;
                obstacle_spawn_timer = random_range(MIN_OBSTACLE_SPAWN_FRAMES, MAX_OBSTACLES_SPAWN_FRAMES);
                break; // Only spawn one per frame
            }
        }
    }
}

void render_game(void) {
    video_box(0, 0, STANDARD_X, GROUND_Y - 2, resample_rgb(db, background_color));
    draw_dino(dino.x, dino.y, game.frame_counter);
    int i;
    for (i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            draw_obstacle(obstacles[i].x, obstacles[i].y, obstacles[i].type);
        }
    }
    draw_score(game.score);
}

void show_start_screen(void) {
    clear_screen();
    video_text(10, 10, "Chrome Dino Game");
    video_text(10, 12, "Press KEY0 to jump");
    video_text(10, 13, "Duck under birds!");
    video_text(10, 14, "Press KEY1 to start");
}

void show_game_over_screen(int final_score) {
    background_color = 0x000000; // Reset background color
    highlights_color = 0xFFFFFF; // Reset highlights color
    inverted_colors = 0; // Reset inverted colors

    clear_screen();
    video_text(12, 10, "GAME OVER!");
    
    char score_text[30] = "Score: ";
    int_to_string(final_score, score_text, 7);
    video_text(10, 12, score_text);
    video_text(8, 14, "Press KEY1 to restart");
}

void int_to_string(int num, char *str, int start_pos) {
    if (num == 0) {
        str[start_pos] = '0';
        str[start_pos + 1] = '\0';
        return;
    }
    
    int i = start_pos;
    while (num > 0) {
        str[i] = (num % 10) + '0';
        num /= 10;
        i++;
    }
    str[i] = '\0';
    
    // Reverse the number part
    int start = start_pos, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void delay(int count) {
    int i;
    for (i = 0; i < count; i++);
}

int random_range(int min, int max) {
    return min + (rand() % (max - min + 1));
}

void video_text(int x, int y, char *text_ptr) {
    volatile char *character_buffer = (char *)FPGA_CHAR_BASE;
    int offset = (y << 7) + x;
    
    while (*text_ptr) {
        *(character_buffer + offset) = *text_ptr;
        text_ptr++;
        offset++;
    }
}

void video_box(int x1, int y1, int x2, int y2, short pixel_color) {
    int pixel_buf_ptr = *(int *)PIXEL_BUF_CTRL_BASE;
    int x_factor = 0x1 << (res_offset + col_offset);
    int y_factor = 0x1 << (res_offset);
    int row;
    int col;

    // Scale coordinates
    x1 /= x_factor;
    x2 /= x_factor;
    y1 /= y_factor;
    y2 /= y_factor;

    for (row = y1; row <= y2; row++) {
        for (col = x1; col <= x2; col++) {
            int pixel_ptr = pixel_buf_ptr + (row << (10 - res_offset - col_offset)) + (col << 1);
            *(short *)pixel_ptr = pixel_color;
        }
    }
}

int resample_rgb(int num_bits, int color) {
    if (num_bits == 8) {
        color = (((color >> 16) & 0x000000E0) | ((color >> 11) & 0x0000001C) |
                 ((color >> 6) & 0x00000003));
        color = (color << 8) | color;
    } else if (num_bits == 16) {
        color = (((color >> 8) & 0x0000F800) | ((color >> 5) & 0x000007E0) |
                 ((color >> 3) & 0x0000001F));
    }
    return color;
}

int get_data_bits(int mode) {
    switch (mode) {
    case 0x0:
        return 1;
    case 0x7:
        return 8;
    case 0x11:
        return 8;
    case 0x12:
        return 9;
    case 0x14:
        return 16;
    case 0x17:
        return 24;
    case 0x19:
        return 30;
    case 0x31:
        return 8;
    case 0x32:
        return 12;
    case 0x33:
        return 16;
    case 0x37:
        return 32;
    case 0x39:
        return 40;
    default:
        return 16;
    }
}

void clear_text_screen(void) {
    volatile char *character_buffer = (char *)FPGA_CHAR_BASE;
    int i;
    
    // Clear the entire character buffer (80 columns x 60 rows)
    for (i = 0; i < 80 * 60; i++) {
        *(character_buffer + i) = ' ';
    }
}
