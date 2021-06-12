#include <Imagine/Imagine.h>

/*
---------------------------------------
 -> Color RGBA Float Data Structure  <- 
--------------------------------------
*/

color_t color(float r, float g, float b, float a)
{
    color_t c = {r, g, b, a};
    return c;
}

color_t unicolor(float f)
{
    color_t c = {f, f, f, 1.0f};
    return c;
}

color_t color_mult(color_t color, float f)
{
    color_t c = color;
    c.r *= f;
    c.g *= f;
    c.b *= f;
    return c;
}

int color_cmp(color_t c1, color_t c2)
{
    return (c1.r == c2.r) * (c1.g == c2.g) * (c1.b == c2.b) * (c1.a == c2.a); 
}

color_t ptoc(pixel_t p)
{
    return color((float)p.r / 255.0f, (float)p.g / 255.0f, (float)p.b / 255.0f, (float)p.a / 255.0f);
}

pixel_t ctop(color_t c)
{
    return pixel((uint8_t)(c.r * 255.0f), (uint8_t)(c.g * 255.0f), (uint8_t)(c.b * 255.0f), (uint8_t)(c.a * 255.0f));
}