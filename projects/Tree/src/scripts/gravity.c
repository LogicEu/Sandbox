#include "../Tree.h"

static const float gravity = 5.0f;

static void gravityApply(Entity e, float deltaTime)
{
    vec2* vel = (vec2*)entity_get(e, COMPONENT_VEL_VEC2);
    bool isGrounded = checkRigidCollision(e, vec2_new(0.0f, vel->y * deltaTime));
    if (!isGrounded) vel->y -= gravity;
    else vel->y = 0.0f;
}

void gravityStep(float deltaTime)
{
    bool* g = (bool*)component_get(COMPONENT_GRAVITY);
    for (unsigned int i = 0; i < component_entity_count(COMPONENT_GRAVITY); i++) {
        if (*g) gravityApply(entity_find(COMPONENT_GRAVITY, i), deltaTime);
        g++;
    }
}