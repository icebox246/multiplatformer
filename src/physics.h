#ifndef __PHYSICS_H
#define __PHYSICS_H

#include "structs.h"

bool aabb_overlaps_aabb(AABB* a, AABB* b);
/* bool entity_resolve_aabb_collision(Entity* entity, AABB* b, bool horz); */

// probably uses global state
/* void entity_resolve_entity_collisions(Entity* entity, bool horz); */
/* void entity_resolve_world_collisions(Entity* entity, bool horz); */

void entity_update(Entity* entity, float dt);
void entity_accelerate(Entity* entity, V2 acc);

#endif  // __PHYSICS_H
