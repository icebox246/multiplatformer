#ifndef __BOX_H
#define __BOX_H

#include "structs.h"
void box_init();
void box_render(Entity* entity);
void box_update(Entity* entity, float dt);

AABB box_aabb(Entity* entity);

#endif  // __BOX_H
