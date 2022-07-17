#ifndef __STRUCTS_H
#define __STRUCTS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "game.h"

#define assert(b) do {if(!(b)) platform_print("assertion_failed: " __FILE__ ":" __LINE__ ":" #b);} while(0)

typedef struct {
    float x, y;
} V2;

typedef struct {
    V2 center;     // center of give AABB
    V2 half_size;  // half size of AABB, because it's more useful for collision
                   // calculation
} AABB;

typedef struct {
    size_t typ;        // enemy type
    size_t state;      // current state of enemy
    size_t s_time;     // time to state change
    size_t health;     // current health
    size_t mx_health;  // maximum health
    bool dead;         // is enemy dead?
    bool on_ground;
	bool pushable;
    V2 pos;  // current position
    V2 vel;  // current velocity
    V2 acc;  // current acceleration, should be zeroed after updating
} Entity;

#define WORLD_SIZE 1024
#define TILE_SIZE 24
#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

typedef struct {
    uint8_t tiles[WORLD_SIZE * WORLD_SIZE];
} World;

typedef enum { ST_RUNNING = 0, ST_PAUSED } GameStateType;

typedef struct {
    bool up, down, left, right, fire;
	bool jp_up;
    V2 mouse_pos;
    V2 mouse_vec;  // relative to center for aiming, etc.
} InputState;

typedef struct {
    V2 center;
} Camera;

#define MAX_ENTITIES 512
typedef struct {
    GameStateType typ;
    World world;
    Entity entities[MAX_ENTITIES];  // player probably sits at 0
    size_t entity_count;           // current enemy count
    InputState input;
    V2 screen_size;
    Camera camera;
	V2 gravity;
} GameState;

#endif  // __STRUCTS_H
