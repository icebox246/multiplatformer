#include "game.h"

#include <stddef.h>

#include "box.h"
#include "entity.h"
#include "globals.h"
#include "la.h"
#include "physics.h"
#include "player.h"
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
    "##.###########################.."
    "##..#############....#########.."
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

    player_init();
    box_init();

    platform_print("Textures loaded!");

    for (int tx = 0; tx < 32; tx++) {
        for (int ty = 0; ty < 16; ty++) {
            st.world.tiles[tx + ty * WORLD_SIZE] =
                (world_temp[tx + ty * 32] == '#');
        }
    }

    for (int tx = 0; tx < WORLD_SIZE; tx++) {
        for (int ty = 40; ty < WORLD_SIZE; ty++) {
            st.world.tiles[tx + ty * WORLD_SIZE] = ty >= 45;
        }
    }

    st.entities[st.entity_count++] =
        (Entity){.typ = E_PLAYER, .pos = v2(40, 40)};

    st.entities[st.entity_count++] =
        (Entity){.typ = E_PLAYER, .pos = v2(80, 40)};

    st.entities[st.entity_count++] =
        (Entity){.typ = E_BOX, .pos = v2(200, 40), .pushable = 1};

    st.gravity = v2(0.0f, 500.0f);
}

void game_update(float dt) { /* platform_print("Tick"); */
    /* Entity* player = &st.entities[0]; */

    for (size_t i = 0; i < st.entity_count; i++) {
        /* if (!st.entities[i].on_ground) { */
        entity_accelerate(&st.entities[i], st.gravity);
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
                platform_blit(x - st.camera.center.x + st.screen_size.x / 2.0f,
                              y - st.camera.center.y + st.screen_size.y / 2.0f,
                              TILE_SIZE, TILE_SIZE, dirt_tex);
            }
        }
    }

    for (size_t i = 0; i < st.entity_count; i++) {
        entity_render(&st.entities[i]);
        /* AABB a = get_entity_aabb(&st.entities[i]); */
        /* platform_rect( */
        /*     a.center.x - a.half_size.x - st.camera.center.x +
         * st.screen_size.x / 2.0f, */
        /*     a.center.y - a.half_size.y - st.camera.center.y +
         * st.screen_size.y / 2.0f, */
        /*     a.half_size.x * 2, a.half_size.y * 2, 0x22ff22ff); */
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
        v2sub(st.input.mouse_pos, v2scale(st.screen_size, 0.5f));
}

void game_resize(size_t x, size_t y) { st.screen_size = v2(x, y); }
