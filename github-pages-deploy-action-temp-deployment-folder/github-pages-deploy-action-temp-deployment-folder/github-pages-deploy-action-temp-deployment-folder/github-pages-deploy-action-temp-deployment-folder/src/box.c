#include "box.h"

#include "game.h"
#include "la.h"
#include "physics.h"
#include "structs.h"
#include "util.h"

int box_texture;

void box_init() { box_texture = platform_load_texture("box.bmp"); }

void box_render(Entity* entity) {
    AABB a = box_aabb(entity);
    platform_blit(TO_SCREEN_V2(v2sub(a.center, a.half_size)), a.half_size.x * 2,
                  a.half_size.y * 2, box_texture);
}

void box_update(Entity* entity, float dt) {
    (void)dt;
    float drag_factor = 1;
    entity_accelerate(entity, v2scale(entity->vel, -drag_factor));
}

AABB box_aabb(Entity* entity) {
    return (AABB){.center = entity->pos, .half_size = v2(8 * 2, 8 * 2)};
}
