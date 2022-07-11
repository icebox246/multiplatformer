#ifndef __GAME_H
#define __GAME_H
#include <stddef.h>

// Exported functions
void game_init(void);            // initializes game state
void game_update(float dt);      // updates game state
void game_render(void);          // renders game state
void game_key_down(size_t key);  // runs when a key is pressed, key is key code
void game_key_up(size_t key);    // runs when a key is released
void game_mouse_move(size_t x, size_t y);  // runs when mouse changes position

// Imported functions

// draws a rectangle on the screen using provided RGBA color
void platform_rect(float x, float y, float w, float h, size_t color);
// prints null-terminated text to some sort of console
void platform_print(char* txt);
// returns random value between 0 and 1
float platform_frand(void);

#endif
