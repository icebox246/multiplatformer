## TODOs:

<!-- TODOS -->

**TODO (1):**

- [ ] add continuous collision detection

**DONE (19):**

- [x] make player render as animated sprite
- [x] draw some player sprites
- [x] make window resizable
- [x] implement creating subtextures
- [x] move camera movement code to render
- [x] make game_update have fixed interval on sdl
- [x] open a window in SDL
- [x] create a canvas and load wasm
- [x] draw a crossplatform rectangle
- [x] implement world holding
- [x] draw some tilemap
- [x] implement aabb physics body
- [x] create a movable camera
- [x] parity with SDL backend
- [x] extract actual resolution of collisons between two aabbs
- [x] use that to implement collision resolution between entities
- [x] implement proper tilemap rendering clamping
- [x] implement loading images
- [x] implement drawing images
<!-- ENDTODOS -->


# WHAT!?!?!?!?!

Fast paced 2d movement shooter.

Demo: [icebox246.github.io/multiplatformer/web/](https://icebox246.github.io/multiplatformer/web/index.html)

# Mechanics

- Fast paced, acceleration based movement.
- Bunny hopping
- Shooting, with chunky feedback (e.g. knockback)

# Setting

- Magical land of ash, and basalt stone temples, and fire (maybe of some cool color, e.g. purple)
- Protagonist is a mage, armed with magic based weapons (wands, magical artefacts)

# Ideas

## Movement

- walking and some reasonable speed
- jumps, which aren't to high
	- acceleration is smaller when jumping
	- boost to speed upon jumping
- crouching
	- character moves slower
	- causes character to fall faster when jumping

## Weapons

- stick for melee combat (like in Heretic)
- a small wand, which by default shoots small projectiles, but can be charged up to produce higher output (a.k.a. DOOM 2016 pistol)
- bigger, maybe dragon-themed wand, shooting a volley of fiery low range projectiles (a.k.a. shotgun)
- some burst fire weapon, shooting 3-4 projectiles in succession
- long range beaming weapon, light and mirror themed
- explosive area of effect weapon, with high knockback (maybe rocket jumping?)


## Enemies

- ghoul or zombie like humanoid, something slow and easy to defeat
- flying charging enemy (similar to lost souls from DOOM)
- pouncing ground enemy (like that thing from Quake)
- flying shooting enemy (like caco from DOOM and that weird thing from Quake), probably very common with multiple variations

## World

- floating islands of gray soil
- sections in dark temples (maybe some fog of war?)

## Presentation

- camera position is interpolated between cursor position and player location (`camera.center = player.pos + mouse_vec * factor;`)
- particles (CPU based)
- screen shake
- chunky sound effects for weapon shooting

# Technical

## Basic structures

- textures are stored as `size_t` because they are indices to some internal textures in back end implementation


```c

typedef struct {
	float x, y;
} vec2;

typedef struct {
	vec2 center;	// center of give AABB
	vec2 half_size; // half size of AABB, because it's more useful for collision calculation
} AABB;


```

## Enemies

- maybe player is internally considered an enemy? 
 - maybe always stays at index 0?
- probably stored in a linear buffer
- all use a common structure, but have custom update functions
- maybe a state based system like in DOOM, but probably can do it non-globally with local enums for each enemy kind
- animations play based on state and velocity (custom calculation per enemy type)
- AABB is acquired based on enemy type

```c
typedef struct {
	size_t typ; // enemy type
	size_t state; // current state of enemy
	size_t s_time; // time to state change
	size_t health; // current health
	size_t mx_health; // maximum health
	bool dead; // is enemy dead?
	bool on_ground;
	vec2 pos; // current position
	vec2 vel; // current velocity
	vec2 acc; // current acceleration, should be zeroed after updating
} Entity;
```

## Projectiles

- again linear buffer and common structure, defining projectile type
- rendering is based of projectile type
- AABB is acquired based on projectile type

```c
typedef struct {
	size_t typ; // projectile type
	vec2 pos; // current position
	vec2 vel; // current velocity
} Projectile;
```

## World
- world mostly has tilemap based collision
- maybe can get away with smaller tiles, to get more flexibility in collision sizes
- maybe some movable entities which have dynamic AABBs
- world stored in a binary format 
	- will probably need an editor of some kind
	- or just an editor mode in the game
- world is structured as an array of indexes to tiles 
	- which have their definitions in some other array

```c
typedef enum {
	TF_COLLISION = 0, // whether the tile has collision
	TF_FOREGROUND, // whether to be drawn in front of entities
} TileFlags;

typedef struct {
	size_t tex; // texture to use for given tile
	size_t flags;  // flags describing given tile
} Tile;

typedef enum {
	ASH = 0,
	BRICK,
	BRICK_BG,
	TILE_TYPE_COUNT
} TileType;

Tile tiles[TILE_TYPE_COUNT] = {
	[ASH] = { .tex = ..., .flags = 1 << TF_COLLISION },
	[BRICK] = {  .tex = ..., .flags = 1 << TF_COLLISION },
	[BRICK_BG] = {  .tex = ... },
}

typedef struct {
	size_t tex;
	AABB aabb;
} WorldEntity;

#define WORLD_SIZE 1024
#define MAX_WORLD_ENTITIES 128

typedef struct {
	uint8_t tiles[WORLD_SIZE * WORLD_SIZE];
	WorldEntity worldEntities[MAX_WORLD_ENTITIES];
} World;
```

## Game state 

```c

typedef enum {
	ST_RUNNING = 0,
	ST_PAUSED
} GameStateType

typedef struct {
	bool up, down, left, right, fire;
	vec2 mouse_pos;
	vec2 mouse_vec; // relative to center for aiming, etc.
} InputState;

typedef struct {
	vec2 center;
} Camera;

# define MAX_ENEMIES 512
typedef struct {
	GameStateType typ;	
	World world;
	Entity enemies[MAX_ENEMIES]; // player probably sits at 0
	size_t enemy_count; // current enemy count
	InputState input;
	vec2 screen_size;
	Camera camera;
} GameState;

// Global Game State instance
GameState st;

```

## Platforms

- native Linux using SDL2 rendering and input
- web assembly using HTML5 canvas rendering, and DOM events for input
	- maaaybe WebGL

- some universal interface (hopefully can just use global variables in WASM context)
	- an event loop implemented per platform calling exported functions

```c

// exported functions
void game_init(); // initializes game state
void game_update(float dt); // updates the game state
void game_render(); // renders game

void game_key_down(size_t key); // runs when a key is pressed, key is key code
void game_key_up(size_t key); // runs when a key is released
void game_button_down(size_t button); // runs when button is pressed
void game_button_up(size_t button); // runs when button is released
void game_mouse_move(size_t x, size_t y); // runs when mouse changes position
void game_resize(size_t w, size_t h); // runs when game view is resized

// imported functions
void platform_rect(float x, float y, float w, float h, size_t color); // draws a rectangle on the screen using provided RGBA color
void platform_rect_rot(float x, float y, float a, float w, float h, size_t color); // draws a rectangle on the screen with provided rotation
void platform_blit(float x, float y, float w, float h, size_t tex); // blits given texture to the screen
void platform_blit_rot(float x, float y, float a, float w, float h, size_t tex); // blits given texture to the screen with rotation
size_t platform_time(); // returns current relative timestamp in millis
size_t platform_prep_text(const char* txt, size_t *w); // returns index of prepared text, optionally returns width
void platform_text(size_t txt); // draws prepared text;
void platform_print(char* txt); // prints null-terminated text to some sort of console

```

## Physics

- basic [x,v,a] movement simulation
- player movement is based on accelerating player in chosen direction and limiting their speed using ground friction which proportional to velocity

```

// probably uses global state
void entity_resolve_entity_collisions(Entity* entity);
void entity_resolve_world_collisions(Entity* entity);

void entity_update(Entity* entity, float dt) {
	// x axis
	entity->pos.x += entity->vel.x * dt;
	entity->vel.x += entity->acc.x * dt;
	entity_resolve_entity_collisions(entity);
	entity_resolve_world_collisions(entity);

	// y axis
	entity->pos.y += entity->vel.y * dt;
	entity->vel.y += entity->acc.y * dt;
	entity_resolve_entity_collisions(entity);
	entity_resolve_world_collisions(entity);

	entity->acc = {};
}

void entity_accelerate(Entity* entity, vec2 acc) {
	entity->acc = v2add(entity->acc, acc);
}

```

