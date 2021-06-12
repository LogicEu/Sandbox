#include "../game.h"

unsigned int point_meeting(entity_t e, float x, float y)
{
    return (x > e.x - absf(e.w) * 0.5f) 
        && (x < e.x + absf(e.w) * 0.5f) 
        && (y > e.y - absf(e.h) * 0.5f) 
        && (y < e.y + absf(e.h) * 0.5f);
}

unsigned int point_meeting_offset(entity_t e, float x, float y, float off_x, float off_y)
{
    x += off_x;
    y += off_y;
    return point_meeting(e, x, y);
}

unsigned int place_meeting(entity_t e1, entity_t e2)
{
    return (e1.x - absf(e1.w) * 0.5f < e2.x + absf(e2.w) * 0.5) &&
        (e1.x + absf(e1.w) * 0.5f > e2.x - absf(e2.w) * 0.5f) &&
        (e1.y - absf(e1.h) * 0.5f < e2.y + absf(e2.h) * 0.5f) &&
        (e1.y + absf(e1.h) * 0.5f > e2.y - absf(e2.h) * 0.5f);
}

unsigned int place_meeting_offset(entity_t e1, entity_t e2, float off_x, float off_y)
{
    e1.x += off_x;
    e1.y += off_y;
    return place_meeting(e1, e2);
}

unsigned int place_meeting_entity_array(entity_t* array, unsigned int size, entity_t e, float off_x, float off_y)
{
    e.x += off_x;
    e.y += off_y;
    for (int i = 0; i < size; i++) {
        if (place_meeting(e, array[i])) return 1;
    }
    return 0;
}