#include "../Tree.h"

static void velocityApply(Entity e, float deltaTime)
{
    vec2* v = (vec2*)entity_get(e, COMPONENT_VEL_VEC2);
    if (vec2_cmp(*v, univec2(0.0f))) return;

    rect_t* rPhi = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
    rect_t* rGl = (rect_t*)entity_get(e, COMPONENT_GL_RECT);

    if (rPhi) {
        rPhi->x += v->x * deltaTime;
        rPhi->y += v->y * deltaTime;
    } if (rGl) {
        rGl->x = rPhi->x;
        rGl->y = rPhi->y;
    }
}

void velocityStep(float deltaTime)
{
    for (unsigned int i = 0; i < component_entity_count(COMPONENT_VEL_VEC2); i++) {
        velocityApply(entity_find(COMPONENT_VEL_VEC2, i), deltaTime);
    }
}