#include "entity.h"

#include "la.h"
#include "structs.h"

AABB get_entity_aabb(Entity* entity) {
    switch (entity->typ) {
        case E_PLAYER: {
            return (AABB){entity->pos, v2(16, 20)};
        } break;
    }

    return (AABB){};
}
