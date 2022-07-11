#include "game.h"

#include <stddef.h>

#include "entity.h"
#include "globals.h"
#include "la.h"
#include "physics.h"
#include "structs.h"

const char world_temp[] =
    "................................"
    "................................"
    "................................"
    "................................"
    "...#########...................."
    "...#########...................."
    "....######......................"
    "................................"
    ".......................####....."
    "................############...."
    ".....................###........"
    "................................"
    "................................"
    "................................"
    "...####################........."
    "....#############..............."
    "\0";

void game_init() {
    platform_print("Hello!");

    for (size_t i = 0; i < 32 * 16; i++) {
        int x = i % 32;
        int y = i / 32;
        st.world.tiles[x + y * WORLD_SIZE] = (world_temp[i] == '#');
    }

    st.entities[st.entity_count++] =
        (Entity){.typ = E_PLAYER, .pos = v2(40, 40)};
}

void game_update(float dt) { /* platform_print("Tick"); */
    V2 gravity = v2(0.0f, 500.0f);
    Entity* player = &st.entities[0];
    {  // player stuff
        float player_move_acc = 1000;
        float player_max_speed = 200;
        float player_jump_speed = 300;
        if (st.input.right) {
            entity_accelerate(player, v2(player_move_acc, 0));
        }
        if (st.input.left) {
            entity_accelerate(player, v2(-player_move_acc, 0));
        }
        // drag
        entity_accelerate(
            player, v2(-player->vel.x * player_move_acc / player_max_speed, 0));
        // jump
        if (st.input.up && player->on_ground) {
            player->vel.y = -player_jump_speed;
        }
    }

    // center camera on player
    st.camera.center = v2add(player->pos, v2scale(st.input.mouse_vec, 0.2f));

    for (size_t i = 0; i < st.entity_count; i++) {
        if (!st.entities[i].on_ground) {
            entity_accelerate(&st.entities[i], gravity);
        }
        entity_update(&st.entities[i], dt);
    }
}

void game_render(void) {
    for (int tx = 0; tx < 32; tx++) {
        for (int ty = 0; ty < 32; ty++) {
            int x = tx * TILE_SIZE;
            int y = ty * TILE_SIZE;
            if (st.world.tiles[tx + ty * WORLD_SIZE]) {
                platform_rect(x - st.camera.center.x + WIDTH / 2.0f,
                              y - st.camera.center.y + HEIGHT / 2.0f, TILE_SIZE,
                              TILE_SIZE, 0xff2222ff);
            }
        }
    }

    for (size_t i = 0; i < st.entity_count; i++) {
        AABB a = get_entity_aabb(&st.entities[i]);
        platform_rect(
            a.center.x - a.half_size.x - st.camera.center.x + WIDTH / 2.0f,
            a.center.y - a.half_size.y - st.camera.center.y + HEIGHT / 2.0f,
            a.half_size.x * 2, a.half_size.y * 2, 0x22ff22ff);
    }
}

void game_key_down(size_t key) {
    switch (key) {
        case 'A':
        case 'a':
            st.input.left = 1;
            break;
        case 'W':
        case 'w':
            st.input.up = 1;
            break;
        case 'D':
        case 'd':
            st.input.right = 1;
            break;
        case 'S':
        case 's':
            st.input.right = 1;
            break;
    }
}
void game_key_up(size_t key) {
    switch (key) {
        case 'A':
        case 'a':
            st.input.left = 0;
            break;
        case 'W':
        case 'w':
            st.input.up = 0;
            break;
        case 'D':
        case 'd':
            st.input.right = 0;
            break;
        case 'S':
        case 's':
            st.input.right = 0;
            break;
    }
}

void game_mouse_move(size_t x, size_t y) {
    st.input.mouse_pos = v2(x, y);
    st.input.mouse_vec =
        v2sub(st.input.mouse_pos, v2(WIDTH * 0.5f, HEIGHT * 0.5f));
}
