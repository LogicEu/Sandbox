#include "Tree.h"
#include "UI/UIcommon.h"

extern vec3 viewport;
extern vec2 mouse;
extern wxDirectory wxDir;

static wxGroup* group;
static bmp_t bmp;
static color_t cursorColor;
static float scale;

typedef enum {
    WX_SE_TITLE_MAIN,
    WX_SE_TITLE_R,
    WX_SE_TITLE_G,
    WX_SE_TITLE_B,
    WX_SE_TITLE_A,

    WX_SE_SLIDER_RED,
    WX_SE_SLIDER_GREEN,
    WX_SE_SLIDER_BLUE,
    WX_SE_SLIDER_ALPHA,

    WX_SE_RECT_BLACK,
    WX_SE_RECT_RED,
    WX_SE_RECT_GREEN,
    WX_SE_RECT_BLUE,
    WX_SE_RECT_YELLOW,
    WX_SE_RECT_CIAN,
    WX_SE_RECT_VIOLET,
    WX_SE_RECT_WHITE,
    WX_SE_RECT_CURSOR
} spriteEditorEnum;

static void slidersReset()
{
    wxSlider* slider;
    slider = group->widgets[WX_SE_SLIDER_RED].widget;
    slider->lerp = cursorColor.r;
    slider = group->widgets[WX_SE_SLIDER_GREEN].widget;
    slider->lerp = cursorColor.g;
    slider = group->widgets[WX_SE_SLIDER_BLUE].widget;
    slider->lerp = cursorColor.b;
    slider = group->widgets[WX_SE_SLIDER_ALPHA].widget;
    slider->lerp = cursorColor.a;
}

static void wxInput()
{
    wxRect* rect = group->widgets[WX_SE_RECT_CURSOR].widget;
    rect->color = cursorColor;

    wxSlider* slider = group->widgets[WX_SE_SLIDER_RED].widget;
    if (slider->selected) {
        cursorColor.r = slider->lerp;
    }
    slider = group->widgets[WX_SE_SLIDER_GREEN].widget;
    if (slider->selected) cursorColor.g = slider->lerp;
    slider = group->widgets[WX_SE_SLIDER_BLUE].widget;
    if (slider->selected) cursorColor.b = slider->lerp;
    slider = group->widgets[WX_SE_SLIDER_ALPHA].widget;
    if (slider->selected) cursorColor.a = slider->lerp;

    if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
        for (unsigned int i = WX_SE_RECT_BLACK; i <= WX_SE_RECT_WHITE; i++) {
            rect = group->widgets[i].widget;
            if (point_meeting(mouse, rect->rect)) {
                memcpy(&cursorColor, &rect->color, sizeof(color_t));
                slidersReset();
                break;
            }
        }
    }
}

static void editorInput()
{
    if (keyboard_pressed(GLFW_KEY_ESCAPE)) {
        systemSetState(STATE_MENU);
    }
    if (keyboard_down(GLFW_KEY_M)) {
        scale += 0.2f;
    }
    if (keyboard_down(GLFW_KEY_N)) {
        scale -= 0.2f;
    }

    if (window_file_has()) {
        bmp_t tmp = bmp_load(window_file_get());
        bmp_t tmp2 = bmp_resize_width(&tmp, 32);
        bmp = bmp_resize_height(&tmp2, 32);
        bmp_free(&tmp);
        bmp_free(&tmp2);
    }

    bool mouseDown = mouse_down(GLFW_MOUSE_BUTTON_LEFT);
    wxGroupUpdate(group, mouse, mouseDown, mouseDown);

    if (mouseDown){
        rect_t r = {
            0.5f * viewport.x / viewport.z, 
            0.5f * viewport.y / viewport.z, 
            (float)bmp.width * scale, 
            (float)bmp.height * scale
        };

        if (point_meeting(mouse, r)) {
            float dx = mouse.x - r.x + r.w * 0.5f;
            float dy = mouse.y - r.y + r.h * 0.5f;
            pixel_t p = ctop(cursorColor);
            void* ptr = px_at(&bmp, (unsigned)(int)clampf(dx / scale, 0.0f, bmp.width), (unsigned)(int)clampf(dy / scale, 0.0f, bmp.height));
            memcpy(ptr, &p, sizeof(pixel_t));
        }
    }
}

static void spriteEditorDraw()
{
    float xscale = viewport.x / viewport.z;
    float yscale = viewport.y / viewport.z;
    bmpDraw(&bmp, xscale * 0.5f, yscale * 0.5f, scale);
    wxGroupDraw(group);
    mouseDraw();
}

static void getInput()
{
    editorInput();
    wxInput();
}

void spriteEditorStep()
{
    getInput();
    spriteEditorDraw();
}

void spriteEditorInit()
{
    group = &wxDir.groups[WX_DIR_SPRITE_EDITOR];
    uint8_t color[] = {255, 255, 255, 255};
    bmp = bmp_color(32, 32, 4, color);
    scale = 2.0f;
    cursorColor = unicolor(0.0f);
    cursorColor.a = 1.0f;
    slidersReset();
}