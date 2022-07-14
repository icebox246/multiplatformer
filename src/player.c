#include "player.h"

#include "game.h"
#include "la.h"
#include "util.h"

size_t player_textures[PS_ST_COUNT];

void player_init() {
    player_textures[0] = platform_load_texture("mage.bmp");
    for (size_t i = 1; i < PS_ST_COUNT; i++) {
        player_textures[i] = platform_create_subtexture(
            player_textures[0], 15 * (i - 1), 0, 15, 23);
    }
}

void player_update(Entity* player, float dt) {
    (void)dt;
    float player_move_acc = 1000;
    float player_max_speed = 250;
    float player_jump_height = 24 * 3;
    float in_air_acc = 300;
    float in_air_max_speed = 500;
    if (!player->on_ground && !st.input.up) {
        player_move_acc = in_air_acc;
        player_max_speed = in_air_max_speed;
    }
    if (st.input.right) {
        entity_accelerate(player, v2(player_move_acc, 0));
    }
    if (st.input.left) {
        entity_accelerate(player, v2(-player_move_acc, 0));
    }
    // drag
    /* if (player->on_ground) { */
    entity_accelerate(
        player, v2(-player->vel.x * player_move_acc / player_max_speed, 0));
    /* } */
    // jump
    if (st.input.jp_up && player->on_ground) {
        const float player_jump_speed =
            qsqrtf(2 * st.gravity.y * player_jump_height);
        player->vel.y = -player_jump_speed;
    }

    int walking = absf(player->vel.x) > 70.f;
    if (player->s_time) player->s_time--;
    if (!player->on_ground) {  // in air
        if (player->vel.y < 0)
            player->state = PS_JUMP;
        else
            player->state = PS_FALL;
    } else if (walking) {  // walking
        if (PS_IS_WALKING(player)) {
            if (!player->s_time) {
                player->state++;
                if (player->state > PS_WALK_3) player->state = PS_WALK_0;
                player->s_time = 10;
            }
        } else {
            player->state = PS_WALK_0;
            player->s_time = 10;
        }
    } else {  // idling
        if (!player->s_time) {
            player->state++;
            player->s_time = 60;
        }
        if (player->state > PS_IDLE_1) {
            player->state = PS_IDLE_0;
        }
    }
}

void player_render(Entity* player) {
    V2 img_pos = v2sub(player->pos, v2(7.5 * 3, 12 * 3));
    platform_blit(TO_SCREEN_V2(img_pos), 15 * 3, 23 * 3, player_textures[player->state]);
}

AABB player_aabb(Entity* entity) {
    return (AABB){.center = entity->pos, .half_size = v2(4.5 * 3, 11 * 3)};
}
