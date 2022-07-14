#include "game.h"

#include <stddef.h>

#include "entity.h"
#include "globals.h"
#include "la.h"
#include "physics.h"
#include "structs.h"
#include "util.h"

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

size_t kitty_tex;
size_t kitty_head_tex;
size_t dirt_tex;

void game_init() {
    platform_print("Hello!");
    platform_print(ftoa(qsqrtf(2)));

    platform_print("Loading textures...");

    kitty_tex = platform_load_texture("kitty.bmp");
    dirt_tex = platform_load_texture("dirt.bmp");

	kitty_head_tex = platform_create_subtexture(kitty_tex, 4, 7, 56, 35);

    platform_print("Textures loaded!");

    for (int tx = 0; tx < 32; tx++) {
        for (int ty = 0; ty < 16; ty++) {
            st.world.tiles[tx + ty * WORLD_SIZE] =
                (world_temp[tx + ty * 32] == '#');
        }
    }

    for (int tx = 0; tx < WORLD_SIZE; tx++) {
        for (int ty = 40; ty < WORLD_SIZE; ty++) {
            st.world.tiles[tx + ty * WORLD_SIZE] = (tx % 20 < 17 || ty >= 45);
        }
    }

    st.entities[st.entity_count++] =
        (Entity){.typ = E_PLAYER, .pos = v2(40, 40)};
}

void game_update(float dt) { /* platform_print("Tick"); */
    V2 gravity = v2(0.0f, 500.0f);
    Entity* player = &st.entities[0];
    {  // player stuff
        float player_move_acc = 1000;
        float player_max_speed = 250;
        float player_jump_height = 24 * 6;
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
                qsqrtf(2 * gravity.y * player_jump_height);
            platform_print(ftoa(2 * gravity.y * player_jump_height));
            platform_print(ftoa(player_jump_speed));
            player->vel.y = -player_jump_speed;
        }
    }

    for (size_t i = 0; i < st.entity_count; i++) {
        /* if (!st.entities[i].on_ground) { */
        entity_accelerate(&st.entities[i], gravity);
        /* } */
        entity_update(&st.entities[i], dt);
    }

    update_input();
}

void game_render(void) {
    // center camera on player
    st.camera.center =
        v2add(st.entities[0].pos, v2scale(st.input.mouse_vec, 0.2f));

    platform_blit(10, 10, 0, 0, kitty_tex);
    platform_blit(10, 100, 0, 0, kitty_head_tex);

    V2 mn, mx;
    {
        AABB a = get_camera_aabb();
        get_aabb_tile_corners(&a, &mn, &mx);
    }
    for (int tx = mn.x; tx < mx.x + 1; tx++) {
        for (int ty = mn.y; ty < mx.y + 1; ty++) {
            int x = tx * TILE_SIZE;
            int y = ty * TILE_SIZE;
            if (st.world.tiles[tx + ty * WORLD_SIZE]) {
                platform_blit(x - st.camera.center.x + WIDTH / 2.0f,
                              y - st.camera.center.y + HEIGHT / 2.0f, TILE_SIZE,
                              TILE_SIZE, dirt_tex);
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
            st.input.jp_up = 1;
            break;
        case 'D':
        case 'd':
            st.input.right = 1;
            break;
        case 'S':
        case 's':
            st.input.down = 1;
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
            st.input.down = 0;
            break;
    }
}

void game_mouse_move(size_t x, size_t y) {
    st.input.mouse_pos = v2(x, y);
    st.input.mouse_vec =
        v2sub(st.input.mouse_pos, v2(WIDTH * 0.5f, HEIGHT * 0.5f));
}
