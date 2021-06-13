#include <Universe/Universe.h>

wxField wxFieldCreate(rect_t rect, unsigned int maxSize)
{
    wxField f;
    f.rect = rect;
    f.text_offset = vec3_new(0.0f, 0.0f, 0.7f);
    f.state = WIDGET_UNSELECTED;
    f.maxSize = maxSize;
    f.strMark = 0;
    memset(f.text, 0, 256);
    return f;
}

void wxFieldUpdate(wxField* field, vec2 mouse, bool pressed)
{
    bool hover = point_meeting(mouse, field->rect);
    if (field->state == WIDGET_SELECTED) {
        if ((!hover && pressed) || keyboard_pressed(GLFW_KEY_ENTER)) {
            field->state = WIDGET_UNSELECTED;
            return;
        }

        char ch = keyboard_char();
        if (keyboard_pressed(GLFW_KEY_ENTER)) {
            field->text[field->strMark++] = '\n';
        }
        if (keyboard_pressed(GLFW_KEY_BACKSPACE) && field->strMark) {
            field->text[--field->strMark] = '\0';
        }
        if (ch != '\0' && field->strMark < field->maxSize) {
            field->text[field->strMark++] = ch;
        }
    } else if (hover) {
        field->state = WIDGET_HOVER;
        if (pressed) field->state = WIDGET_SELECTED;
    }
    else field->state = WIDGET_UNSELECTED;
}

void wxFieldDraw(wxField* field)
{   
    color_t box = globalTheme.primary[field->state];
    color_t font = globalTheme.secondary[field->state];
    
    wxRect rect = {field->rect, box};
    wxRectDraw(rect);

    vec2 fontPos = {field->rect.x - field->rect.w * 0.5f + field->text_offset.x, field->rect.y - field->rect.h * 0.5f + field->text_offset.y};
    fontDrawText(globalFont, field->text, fontPos.x, fontPos.y, (field->rect.h / 32.0f) * field->text_offset.z , vec3_new(font.r, font.g, font.b));
}
