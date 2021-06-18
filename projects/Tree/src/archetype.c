#include "Tree.h"

static const vec2 vecZero = {0.0f, 0.0f};
static const bool bTrue = true, bFalse = false;
static const float fOne = 1.0f;

extern vec2 spawnPoint;

Entity archetypePlayer()
{
    static unsigned int k = SPRITE_KID_STANDING;

    sprite_t* s = assetsGetSprite(k);
    rect_t r = {spawnPoint.x, spawnPoint.y, s->textures->width, s->textures->height};
    return archetype_entity(ARCHETYPE_PLAYER, 5, &k, &r, &r, &vecZero, &bTrue);
}

Entity archetypeTerrainTile(unsigned int texture_index, vec2 position)
{
    static bool rc = true;

    texture_t* t = assetsGetTexture(texture_index);
    rect_t r = {position.x, position.y, t->width, t->height};
    return archetype_entity(ARCHETYPE_TERRAIN, 4, &t->id, &r, &r, &rc);
}

Entity archetypeBullet(vec2 position, vec2 vel, BulletKind kind)
{
    texture_t* t;
    if (kind == BULLET_KIND_ROCKET) t = assetsGetTexture(TEXTURE_ROCKET);
    else t = assetsGetTexture(TEXTURE_REDBALL);
    rect_t r = {position.x, position.y, t->width, t->height};
    float f = vec2_to_rad(vel);
    return archetype_entity(ARCHETYPE_BULLET, 6, &t->id, &r, &r, &vel, &kind, &f);
}

Entity archetypeBlast(vec2 position, float explotionForce)
{   
    texture_t* t = assetsGetTexture(TEXTURE_EXPLOTION);
    rect_t r = {position.x, position.y, t->width, t->height};
    float rot = (float)(rand() & 66 / 10);
    return archetype_entity(ARCHETYPE_BLAST, 8, &t->id, &r, &r, &vecZero, &fOne, &fOne, &rot, &explotionForce);
}

Entity archetypeFire(vec2 position, vec2 vel)
{   
    static float force = 20.0f;

    texture_t* t = assetsGetTexture(TEXTURE_FIRE);
    rect_t r = {position.x, position.y, t->width, t->height};
    float rot = vec2_to_rad(vel);
    return archetype_entity(ARCHETYPE_BLAST, 8, &t->id, &r, &r, &vel, &fOne, &fOne, &rot, &force);
}

Entity archetypeSmoke(vec2 position, unsigned int textureIndex)
{
    static float s = 0.5f;

    texture_t* t = assetsGetTexture(textureIndex);
    rect_t r = {position.x, position.y, t->width, t->height};
    float rot = (float)(rand() & 66 / 10);
    return archetype_entity(ARCHETYPE_SMOKE, 6, &t->id, &r, &fOne, &s, &rot, &bTrue);
}

Entity archetypeShadow(vec2 position, float side)
{
    static float a = 0.6f;

    texture_t* t = assetsGetSprite(SPRITE_KID_JUMPING)->textures;
    rect_t r = {position.x, position.y, t->width * (-1.0f + 2.0f * (float)(side > 0.0f)), t->height};
    return archetype_entity(ARCHETYPE_SHADOW, 4, &t->id, &r, &a, &bTrue);
}

Entity archetypeFirebarrel(vec2 position)
{
    texture_t* t = assetsGetTexture(TEXTURE_FIREBARREL);
    rect_t r = {position.x, position.y, t->width, t->height};
    return archetype_entity(ARCHETYPE_FIREBARREL, 6, &t->id, &r, &r, &vecZero, &bTrue, &bFalse);
}

Entity archetypeJetpackController(vec2 position)
{
    static unsigned int fuel = 300;

    texture_t* t = assetsGetTexture(TEXTURE_JETPACK);
    rect_t r = {position.x, position.y, t->width, t->height};
    return archetype_entity(ARCHETYPE_JETPACK, 7, &t->id, &r, &r, &vecZero, &bTrue, &bFalse, &fuel);
}

Entity archetypeGranade(vec2 position)
{
    static GranadeComponent gc = {GRANADE_LOOSE, 1.0f};

    texture_t* t = assetsGetTexture(TEXTURE_GRANADE);
    rect_t r = {position.x, position.y, t->width, t->height};
    return archetype_entity(ARCHETYPE_GRANADE, 6, &t->id, &r, &r, &vecZero, &bTrue, &gc);
}

void archetypesInit()
{
    // Player
    archetype_create(5, COMPONENT_SPRITE_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_GRAVITY);
    // Tile
    archetype_create(4, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_RIGID_COLLIDER);
    // Gun
    archetype_create(8, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_GRAVITY, COMPONENT_GUN_CONTROLLER, COMPONENT_ROTATION);
    // Bullet
    archetype_create(6, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_BULLET, COMPONENT_ROTATION);
    // Blast 
    archetype_create(8, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_ALPHA, COMPONENT_SCALE, COMPONENT_ROTATION, COMPONENT_EXPLOTION);
    // Smoke
    archetype_create(6, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_ALPHA, COMPONENT_SCALE, COMPONENT_ROTATION, COMPONENT_SMOKE);
    // Shadow
    archetype_create(4, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_ALPHA, COMPONENT_SHADOW);
    // Firebarrel
    archetype_create(6, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_GRAVITY, COMPONENT_FIREBARREL);
    // Jetpack+
    archetype_create(7, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_GRAVITY, COMPONENT_JETPACK, COMPONENT_AMMO);
    // Granade
    archetype_create(6, COMPONENT_TEX_ID, COMPONENT_GL_RECT, COMPONENT_PHI_RECT, COMPONENT_VEL_VEC2, COMPONENT_GRAVITY, COMPONENT_GRANADE);
}