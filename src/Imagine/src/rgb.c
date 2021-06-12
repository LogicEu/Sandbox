#include <Imagine/Imagine.h>

rgb_t rgb(uint8_t r, uint8_t g, uint8_t b)
{
    rgb_t rgb = {r, g, b};
    return rgb;
}

rgb_t unirgb(uint8_t i)
{
    rgb_t p = {i, i, i};
    return p;
}

int rgbcmp(rgb_t p1, rgb_t p2)
{
    return (p1.r == p2.r) * (p1.g == p2.g) *  (p1.b == p2.b);
}