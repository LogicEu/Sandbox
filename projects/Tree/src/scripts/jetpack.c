#include "../Tree.h"

extern Entity player;

static void jetpackApply(Entity e)
{
    rect_t* playerGl = (rect_t*)entity_get(player, COMPONENT_GL_RECT);
    rect_t* rPhi = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
    rect_t* rTex = (rect_t*)entity_get(e, COMPONENT_GL_RECT);

    if ((playerGl->w > 0.0f && rTex->w > 0.0f) ||
        (playerGl->w < 0.0f && rTex->w < 0.0f)) {
        rTex->w *= -1.0f;
    } 

    rPhi->x = playerGl->x + rTex->w * 0.3f;      
    rPhi->y = playerGl->y - 4.0f;
    rTex->x = rPhi->x;
    rTex->y = rPhi->y; 
    rTex->w *= -1.0f;
}

void jetpackUse(Entity jetpack)
{
    if (!*(bool*)entity_get(jetpack, COMPONENT_JETPACK)) return;
    unsigned int* fuel = (unsigned int*)entity_get(jetpack, COMPONENT_AMMO);
    if (!*fuel) return;
    vec2* playerVel = (vec2*)entity_get(player, COMPONENT_VEL_VEC2);
    vec2 pos = *(vec2*)entity_get(jetpack, COMPONENT_PHI_RECT);
    smokeEmit(pos, TEXTURE_SMOKE2);
    playerVel->y += 8.0f;
    *fuel -= 1;
}

void jetpackPick(Entity jetpack)
{
    bool t = true;
    memset(entity_get(jetpack, COMPONENT_VEL_VEC2), 0, sizeof(vec2));
    memset(entity_get(jetpack, COMPONENT_GRAVITY), 0, sizeof(bool));
    entity_set(jetpack, COMPONENT_JETPACK, &t);
}

void jetpackDrop(Entity jetpack)
{
    bool t = true;
    memset(entity_get(jetpack, COMPONENT_JETPACK), 0, sizeof(bool));
    entity_set(jetpack, COMPONENT_GRAVITY, &t);
}

void jetpackControllerStep()
{
    unsigned int count = component_entity_count(COMPONENT_JETPACK);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_JETPACK, i);
        bool activated = *(bool*)entity_get(e, COMPONENT_JETPACK);
        if (activated) jetpackApply(e);
    }
}