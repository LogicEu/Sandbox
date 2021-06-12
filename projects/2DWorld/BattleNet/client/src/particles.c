#include "../BNGame.h"

void emit_smoke(list_t* smoke, sprite_t* smoke_sprite, float x, float y)
{
    entity_t e = entity_new(smoke_sprite->textures->id, x, y, smoke_sprite->textures->width, smoke_sprite->textures->height);
    e.velocity.x = 1.0f;
    e.scale = 0.5f;
    e.rot = (float)(rand() % 66) / 10.0f;
    list_push(smoke, &e);
}

void emit_big_smoke(list_t* smoke, sprite_t* smoke_sprite, float x, float y)
{
    entity_t e = entity_new(smoke_sprite->textures->id, x, y, smoke_sprite->textures->width, smoke_sprite->textures->height);
    e.velocity.x = 1.0f;
    e.scale = 4.0f;
    e.rot = (float)(rand() % 66) / 10.0f;
    list_push(smoke, &e);
}

void emit_blood(list_t* blood, sprite_t* blood_sprite, unsigned int count, float x, float y)
{
    for (int i = 0; i < count; i++) {
        entity_t e = entity_new(blood_sprite->textures->id, x, y, blood_sprite->textures->width, blood_sprite->textures->height);
        float rot = (float)(rand() % 33) / 10.0f;
        e.velocity.x = cosf(rot) * blood_force;
        e.velocity.y = sinf(rot) * blood_force;
        e.scale = (float)(rand() % 100) / 100.0f;
        e.rot = rot;
        list_push(blood, &e);
    }
}

void emit_shadow(list_t* shadows, sprite_t* sprite, float x, float y, float rot)
{
    entity_t e = entity_new(sprite->textures->id, x, y, sprite->textures->width, sprite->textures->height);
    e.velocity.x = 0.5f;
    e.velocity.y = rot;
    list_push(shadows, &e);
}

void emit_explotion(list_t* explotions, sprite_t* exp_sprite, float x, float y, float exp_force)
{
    entity_t exp = entity_new(exp_sprite->textures->id, x, y, exp_sprite->textures->width, exp_sprite->textures->height);
    exp.velocity.x = 1.0f;
    exp.velocity.y = exp_force;
    list_push(explotions, &exp);
}