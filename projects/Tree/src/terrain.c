#include "Tree.h"

extern unsigned int randSeed;

void terrainReduce()
{
    for (Entity e = 1; e < entity_count(); e++) {
        void* r = entity_get(e, COMPONENT_GL_RECT);
        if (r == NULL) continue;
        for (Entity ee = e + 1; ee < entity_count(); ee++) {
            void* rr = entity_get(ee, COMPONENT_GL_RECT);
            if (rr == NULL) continue;
            if (!memcmp(r, rr, sizeof(rect_t))) {  
                entity_destroy(ee);
            }
        }
    }
}

void terrainRecalcSingleTexture(Entity e)
{
    static const vec2 off = {0.0f, 32.0f};

    unsigned int* t = (unsigned int*)entity_get(e, COMPONENT_TEX_ID);
    bool change = checkRigidCollision(e, off);
    if  (change && *t == assetsGetTexture(TEXTURE_TILE_GRASS)->id) {
        *t = assetsGetTexture(TEXTURE_TILE_DIRT)->id;
    }
    if (!change && *t == assetsGetTexture(TEXTURE_TILE_DIRT)->id) {
        *t = assetsGetTexture(TEXTURE_TILE_GRASS)->id;
    }
}

void terrainRecalcTextures()
{
    const unsigned int component_count = component_entity_count(COMPONENT_RIGID_COLLIDER);
    for (unsigned int i = 0; i < component_count; i++) {
        terrainRecalcSingleTexture(entity_find(COMPONENT_RIGID_COLLIDER, i));
    }  
}

void terraingGenRand(unsigned int width, unsigned int height)
{
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            if (rand_next() % 100 > 90) {
                archetypeTerrainTile(TEXTURE_TILE_GRASS, vec2_new((x - width / 2) * 32.0f + 16.0f, (y - height / 2) * 32.0f + 16.0f));
            }
        }
    }
}