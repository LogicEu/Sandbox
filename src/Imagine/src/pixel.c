#include <Imagine/Imagine.h>

/*
-----------------------------------------
 -> Pixel RGBA uint8_t Data Structure  <- 
-----------------------------------------
*/

pixel_t pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    pixel_t p = {r, g, b, a};
    return p;
}

pixel_t unipixel(uint8_t i)
{
    pixel_t p = {i, i, i, 255};
    return p;
}

int pixel_cmp(pixel_t p1, pixel_t p2)
{
    return (p1.r == p2.r) * (p1.g == p2.g) *  (p1.b == p2.b) * (p1.a == p2.a);
}