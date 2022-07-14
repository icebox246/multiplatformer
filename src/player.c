#include "player.h"

void player_update(Entity* player, float dt) {
	(void) dt;
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
}
