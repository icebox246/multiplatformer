#include "entity.h"

#include "la.h"
#include "structs.h"
#include "player.h"

AABB get_entity_aabb(Entity* entity) {
    switch (entity->typ) {
        case E_PLAYER: {
            return player_aabb(entity);
        } break;
    }

    return (AABB){};
}

void entity_render(Entity* entity) {
    switch (entity->typ) {
        case E_PLAYER: {
            player_render(entity);
        } break;
    }
}
