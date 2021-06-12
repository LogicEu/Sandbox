#include <Universe/Universe.h>

wxTitle wxTitleCreate(const char* str, vec3 pos_scale, color_t color)
{
    wxTitle t;
    strcpy(t.string, str);
    t.pos_scale = pos_scale;
    t.color = color;
    return t;
}

void wxTitleDraw(wxTitle* title)
{
    fontDrawText(globalFont, title->string, title->pos_scale.x, title->pos_scale.y, title->pos_scale.z , *(vec3*)&title->color);
}