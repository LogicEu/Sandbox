#include "../BNGame.h"

void player_respawn(entity_t* player, packet_t* user, texture_t* t)
{
    player->rot = 0.0f;
    player->rect.x = SCREEN_XSCALE / 2 - 10;
    player->rect.y = SCREEN_YSCALE / 2 - 20;
    player->velocity.x = 0.0f;
    player->velocity.y = 0.0f;
    player->rect.w = (float)t->width * 0.5f;
    player->rect.h = (float)t->height;
    user->index = KID_STANDING;
    user->count = IM_NOTHING;
    user->state = STATE_LIVE;
    user->hp = start_hp;
}