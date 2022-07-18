#ifndef __ENTITY_H
#define __ENTITY_H

#include "structs.h"

typedef enum {
	E_PLAYER,
	E_BOX
} EntityType;

AABB get_entity_aabb(Entity* entity);
void entity_render(Entity* entity);

#endif  // __ENTITY_H
