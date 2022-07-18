#include "physics.h"

#include <stddef.h>

#include "box.h"
#include "entity.h"
#include "game.h"
#include "globals.h"
#include "la.h"
#include "player.h"
#include "structs.h"
#include "util.h"

bool entity_resolve_aabb_collision(Entity* entity, AABB* b, bool axis) {
    AABB a = get_entity_aabb(entity);
    if (aabb_overlaps_aabb(&a, b)) {
        float d = absf(V2AXIS(a.center, axis) - V2AXIS(b->center, axis)) -
                  V2AXIS(a.half_size, axis) - V2AXIS(b->half_size, axis);
        if (V2AXIS(a.center, axis) < V2AXIS(b->center, axis))
            V2AXIS(entity->pos, axis) += d;
        else
            V2AXIS(entity->pos, axis) -= d;
        V2AXIS(entity->vel, axis) = 0;
        if (axis == 1 && entity->vel.y >= 0) {
            entity->on_ground = 1;
        }

        return 1;
    }
    return 0;
}

bool entity_resolve_entity_collision(Entity* entity, Entity* other, bool axis) {
    float push_force = 100;
    AABB a = get_entity_aabb(entity);
    AABB b = get_entity_aabb(other);
    if (aabb_overlaps_aabb(&a, &b)) {
        float d = absf(V2AXIS(a.center, axis) - V2AXIS(b.center, axis)) -
                  V2AXIS(a.half_size, axis) - V2AXIS(b.half_size, axis);
        if (V2AXIS(a.center, axis) < V2AXIS(b.center, axis))
            V2AXIS(entity->pos, axis) += d;
        else
            V2AXIS(entity->pos, axis) -= d;
        if (!other->pushable)
            V2AXIS(entity->vel, axis) = 0;
        else {
            entity_accelerate(other, v2scale(v2norm(entity->vel), push_force));
            V2AXIS(entity->vel, axis) = clampf(V2AXIS(entity->vel, axis),
                                               -absf(V2AXIS(other->vel, axis)),
                                               absf(V2AXIS(other->vel, axis)));
        }
        if (axis == 1 && entity->vel.y >= 0) {
            entity->on_ground = 1;
        }

        return 1;
    }
    return 0;
}

bool aabb_overlaps_aabb(AABB* a, AABB* b) {
    if (absf(a->center.x - b->center.x) < a->half_size.x + b->half_size.x &&
        absf(a->center.y - b->center.y) < a->half_size.y + b->half_size.y)
        return 1;
    return 0;
}

void entity_resolve_entity_collisions(Entity* entity, float dt, bool axis) {
    (void)dt;
    for (size_t i = 0; i < st.entity_count; i++) {
        if (&st.entities[i] == entity) continue;
        /* AABB a = get_entity_aabb(entity); */
        /* float v = V2AXIS(entity->vel, axis); */
        entity_resolve_entity_collision(entity, &st.entities[i], axis);
    }
}

void entity_resolve_world_collisions(Entity* entity, bool axis) {
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

            entity_resolve_aabb_collision(entity, &b, axis);
        }
    }
}

void entity_update(Entity* entity, float dt) {
    switch (entity->typ) {
        case E_PLAYER: {
            player_update(entity, dt);
        } break;
        case E_BOX: {
            box_update(entity, dt);
        } break;
    }
    // x axis
    entity->vel.x += entity->acc.x * dt;
    entity->pos.x += entity->vel.x * dt;
    entity_resolve_entity_collisions(entity, dt, 0);
    entity_resolve_world_collisions(entity, 0);

    // y axis
    entity->vel.y += entity->acc.y * dt;
    entity->pos.y += entity->vel.y * dt;

    if (entity->vel.y < 0) entity->on_ground = 0;

    entity_resolve_entity_collisions(entity, dt, 1);
    entity_resolve_world_collisions(entity, 1);

    entity->acc = v2zero();
}

void entity_accelerate(Entity* entity, V2 acc) {
    entity->acc = v2add(entity->acc, acc);
}
