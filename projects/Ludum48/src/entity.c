#include "../game.h"

entity_t entity(unsigned int id, float x, float y, float w, float h)
{
    entity_t e = {id, x, y, w, h};
    return e;
}