#include "physics.h"

#include <stddef.h>

#include "entity.h"
#include "game.h"
#include "globals.h"
#include "la.h"
#include "structs.h"
#include "util.h"

void entity_resolve_aabb_collision(Entity* entity, AABB* b, bool horz) {
    AABB a = get_entity_aabb(entity);
    if (aabb_overlaps_aabb(&a, b)) {
        if (horz) {
            // moving horz
            float dx =
                absf(a.center.x - b->center.x) - a.half_size.x - b->half_size.x;
            if (a.center.x < b->center.x)
                entity->pos.x += dx;
            else
                entity->pos.x -= dx;
            entity->vel.x = 0;
        } else {
            // moving vert
            float dy =
                absf(a.center.y - b->center.y) - a.half_size.y - b->half_size.y;
            if (a.center.y < b->center.y)
                entity->pos.y += dy;
            else
                entity->pos.y -= dy;
            if (entity->vel.y >= 0) {
                entity->on_ground = 1;
            }
            entity->vel.y = 0;
        }
    }
}

bool aabb_overlaps_aabb(AABB* a, AABB* b) {
    if (absf(a->center.x - b->center.x) < a->half_size.x + b->half_size.x &&
        absf(a->center.y - b->center.y) < a->half_size.y + b->half_size.y)
        return 1;
    return 0;
}

void entity_resolve_entity_collisions(Entity* entity, bool horz) {
    for (size_t i = 0; i < st.entity_count; i++) {
        if (&st.entities[i] == entity) continue;
        /* AABB a = get_entity_aabb(entity); */
        AABB b = get_entity_aabb(&st.entities[i]);
        entity_resolve_aabb_collision(entity, &b, horz);
    }
}

void entity_resolve_world_collisions(Entity* entity, bool horz) {
    AABB a = get_entity_aabb(entity);
    V2 mn, mx;
    get_aabb_tile_corners(&a, &mn, &mx);

    for (size_t y = mn.y; y < mx.y + 1; y++) {
        for (size_t x = mn.x; x < mx.x + 1; x++) {
            size_t i = x + y * WORLD_SIZE;
            if (!st.world.tiles[i]) continue;
            /* AABB a = get_entity_aabb(entity); */
            AABB b = (AABB){v2(x * TILE_SIZE + TILE_SIZE / 2.0f,
                               y * TILE_SIZE + TILE_SIZE / 2.0f),
                            v2(TILE_SIZE / 2.0f, TILE_SIZE / 2.0f)};

            entity_resolve_aabb_collision(entity, &b, horz);
        }
    }
}

void entity_update(Entity* entity, float dt) {
    // x axis
    entity->pos.x += entity->vel.x * dt;
    entity->vel.x += entity->acc.x * dt;
    entity_resolve_entity_collisions(entity, 1);
    entity_resolve_world_collisions(entity, 1);

    // y axis
    entity->pos.y += entity->vel.y * dt;
    entity->vel.y += entity->acc.y * dt;

    if (entity->vel.y < 0) entity->on_ground = 0;

    entity_resolve_entity_collisions(entity, 0);
    entity_resolve_world_collisions(entity, 0);

    entity->acc = v2zero();
}

void entity_accelerate(Entity* entity, V2 acc) {
    entity->acc = v2add(entity->acc, acc);
}
