#include <Photon/Photon.h>

/*
-------------
 -> Entity <- 
-------------
*/

entity_t entity_new(unsigned int id, float x, float y, float width, float height)
{
    entity_t e;
    e.id = id;
    e.rect = rect_new(x, y, width * 0.5f, height * 0.5f);
    e.velocity = univec2(0.0f);
    e.scale = 1.0f;
    e.rot = 0.0f;
    return e;
}