#ifndef __ENTITY_H
#define __ENTITY_H

#include "structs.h"

typedef enum {
	E_PLAYER
} EntityType;

AABB get_entity_aabb(Entity* entity);

#endif  // __ENTITY_H
