#include "../Tree.h"

bool checkRigidCollision(Entity entity, vec2 off)
{
    rect_t* rPhiPtr = (rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    rect_t rPhi = *rPhiPtr;
    rPhi.x += off.x;
    rPhi.y += off.y;
    const unsigned int count = component_entity_count(COMPONENT_RIGID_COLLIDER);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_RIGID_COLLIDER, i);
        rect_t* r = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (r == rPhiPtr) continue;
        if (place_meeting(rPhi, *r)) return true;
    }
    return false;
}

Entity checkGunCollision(Entity entity)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    const unsigned int count = component_entity_count(COMPONENT_GUN_CONTROLLER);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_GUN_CONTROLLER, i);
        if (*(bool*)entity_get(e, COMPONENT_GUN_CONTROLLER)) continue;
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkJetpackCollision(Entity entity)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    const unsigned int count = component_entity_count(COMPONENT_JETPACK);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_JETPACK, i);
        if (*(bool*)entity_get(e, COMPONENT_JETPACK)) continue;
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkFirebarrelCollision(Entity entity)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    const unsigned int count = component_entity_count(COMPONENT_FIREBARREL);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_FIREBARREL, i);
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkGranadeCollision(Entity entity)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    const unsigned int count = component_entity_count(COMPONENT_GRANADE);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_GRANADE, i);
        if (!((GranadeComponent*)entity_get(e, COMPONENT_GRANADE))->state) continue;
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkPhiCollision(Entity entity, Component component)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    unsigned int count = component_entity_count(component);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(component, i);
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkPhiScaledCollision(Entity entity, Component component)
{
    rect_t rPhi = *(rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    unsigned int count = component_entity_count(component);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(component, i);
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        float scale = *(float*)entity_get(e, COMPONENT_SCALE);
        r.w *= scale;
        r.h *= scale;
        if (place_meeting(rPhi, r)) return e;
    }
    return 0;
}

Entity checkGlCollision(Entity entity, Component component)
{
    rect_t rGl = *(rect_t*)entity_get(entity, COMPONENT_GL_RECT);
    unsigned int count = component_entity_count(component);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(component, i);
        rect_t r = *(rect_t*)entity_get(e, COMPONENT_GL_RECT);
        if (place_meeting(rGl, r)) return e;
    }
    return 0;
}