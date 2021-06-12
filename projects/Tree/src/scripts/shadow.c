#include "../Tree.h"

static float shadowAlarm = 1.0f;

void shadowEmit(vec2 position, float side)
{
    if (shadowAlarm < 0.0f) {
        archetypeShadow(position, side);
        shadowAlarm = 1.0f;
    }
}

void shadowStep(float deltaTime)
{
    static const float shadowAlarmRate = 50.0f;
     if (shadowAlarm >= 0.0f) shadowAlarm -= deltaTime * shadowAlarmRate;
    unsigned int count = component_entity_count(COMPONENT_SHADOW);
    for (unsigned int i = 0; i < count; i++) {
        Entity e = entity_find(COMPONENT_SHADOW, i);
        float* alpha = (float*)entity_get(e, COMPONENT_ALPHA);
        *alpha -= deltaTime;
        if (*alpha < 0.0f) {
            entity_destroy(e);
            i--;
            count--;
        }
    }
}