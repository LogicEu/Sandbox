#include <Photon/Photon.h>

/*
-----------
 -> Rect <- 
-----------
*/

rect_t rect_new(float x, float y, float w, float h)
{
    rect_t r = {x, y, w, h};
    return r;
}

void rect_move(rect_t* r, vec2 add)
{
    r->x += add.x;
    r->y += add.y;
}