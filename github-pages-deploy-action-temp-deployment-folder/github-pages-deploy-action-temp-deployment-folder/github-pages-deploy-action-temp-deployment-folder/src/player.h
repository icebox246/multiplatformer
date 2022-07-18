#ifndef __PLAYER_H
#define __PLAYER_H

#include "structs.h"
#include "physics.h"
#include "la.h"
#include "game.h"

typedef enum {
	PS_WHOLE = 0,
	PS_IDLE_0,
	PS_IDLE_1,
	PS_WALK_0,
	PS_WALK_1,
	PS_WALK_2,
	PS_WALK_3,
	PS_JUMP,
	PS_FALL,
	PS_ST_COUNT
} PlayerStates;

#define PS_IS_WALKING(p) ((p)->state >= PS_WALK_0 && (p)->state <= PS_WALK_3)

void player_init();

void player_update(Entity* player, float dt);
void player_render(Entity* player);

AABB player_aabb(Entity* entity);

#endif // __PLAYER_H
