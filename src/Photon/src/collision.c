#include <Photon/Photon.h>
#include <stdio.h>

/*
-----------------
 -> Collisions <- 
-----------------
*/

static float absf(float f)
{
    return f * (float)(f >= 0.0f) - f * (float)(f < 0.0f);
}

static rect_t rect_half(rect_t r)
{
    rect_t ret = {r.x, r.y, absf(r.w) * 0.5f, absf(r.h) * 0.5f};
    return ret;
}

bool point_meeting(vec2 v, rect_t r)
{
    r = rect_half(r);
    return  (v.x > r.x - r.w) &&
            (v.x < r.x + r.w) &&
            (v.y > r.y - r.h) &&
            (v.y < r.y + r.h);
}

bool point_meeting_offset(vec2 v, rect_t r, vec2 offset)
{
    v = vec2_add(v, offset);
    return point_meeting(v, r);
}

bool point_meeting_array(vec2 v, rect_t* array, unsigned int size)
{
    for (rect_t* end = array + size; array != end; array++) {
        if (point_meeting(v, *array)) return true;
    }
    return false;
}

bool point_meeting_array_offset(vec2 v, rect_t* array, unsigned int size, vec2 off)
{
    v = vec2_add(v, off);
    for (rect_t* end = array + size; array != end; array++) {
        if (point_meeting(v, *array)) return true;
    }
    return false;
}

bool place_meeting(rect_t r1, rect_t r2)
{
    r1 = rect_half(r1);
    r2 = rect_half(r2);
    return  (r1.x - r1.w < r2.x + r2.w) &&
            (r1.x + r1.w > r2.x - r2.w) &&
            (r1.y - r1.h < r2.y + r2.h) &&
            (r1.y + r1.h > r2.y - r2.h);
}

bool place_meeting_offset(rect_t r1, rect_t r2, vec2 offset)
{
    rect_move(&r1, offset);
    return place_meeting(r1, r2);
}

bool place_meeting_array(rect_t* array, rect_t* rect, unsigned int size)
{
    rect_t r = *rect;
    for (rect_t* end = array + size; array != end; array++) {
        if (rect == array) continue;
        if (place_meeting(*array, r)) return true;
    }
    return false;
}

bool place_meeting_array_offset(rect_t* array, rect_t* rect, unsigned int size, vec2 off)
{
    rect_t r = {rect->x + off.x, rect->y + off.y, rect->w, rect->h};
    for (rect_t* end = array + size; array != end; array++) {
        if (rect == array) continue;
        if (place_meeting(*array, r)) return true;
    }
    return false;
}

bool place_meeting_entity_array(entity_t* array, unsigned int size, entity_t* entity)
{
    rect_t r1 = entity->rect;
    for (entity_t* end = array + size; array != end; array++) {
        if (place_meeting(r1, array->rect)) return true;
    }
    return false;
}

bool place_meeting_entity_array_offset(entity_t* array, unsigned int size, entity_t* entity, vec2 offset)
{
    rect_t r1 = entity->rect;
    rect_move(&r1, offset);
    for (entity_t* end = array + size; array != end; array++) {
        if (place_meeting(r1, array->rect)) return true;
    }
    return false;
}
