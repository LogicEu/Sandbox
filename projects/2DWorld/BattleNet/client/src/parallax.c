#include "../BNGame.h"

void draw_parallax(array_t* images, vec2 cam, vec2 center)
{
    color_t c = color(1.0f, 1.0f, 1.0f, 1.0f);
    texture_t* t = (texture_t*)images->data + 2;
    float x = cam.x + t->width * 0.33f / core.scale;
    draw_texture_transformed(texshader, t, c, x / 1.5f + center.x, cam.y / 1.015f + center.y, (float)t->width, (float)t->height, 0.3f, 0.0f);
    t--;
    draw_texture_transformed(texshader, t, c, x / 3.0f + center.x, cam.y / 1.03f + center.y, (float)t->width, (float)t->height, 0.3f, 0.0f);
    t--;
    draw_texture_transformed(texshader, t, c, x / 4.5f + center.x, cam.y / 1.045f + center.y, (float)t->width, (float)t->height, 0.3f, 0.0f);
}