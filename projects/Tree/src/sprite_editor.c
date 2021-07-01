#include "Tree.h"
#include "UI/UIcommon.h"

extern vec3 viewport;
extern vec2 mouse;
extern wxDirectory wxDir;
extern unsigned int currentPlayerSprite;
extern Entity player;

static wxGroup* group;
bmp_t bmp;
static color_t cursorColor;
static float scale;
static unsigned int fieldWidth, fieldHeight;

#define SCALE_MULT 4.0f

typedef enum {
    WX_SE_TITLE_MAIN,
    WX_SE_TITLE_R,
    WX_SE_TITLE_G,
    WX_SE_TITLE_B,
    WX_SE_TITLE_A,
    WX_SE_TITLE_ZOOM,

    WX_SE_BUTTON_MENU,
    WX_SE_BUTTON_SAVE,
    WX_SE_BUTTON_LOAD,
    WX_SE_BUTTON_NEW,
    WX_SE_BUTTON_SUBMIT,
    
    WX_SE_FIELD_DIR,
    WX_SE_FIELD_WIDTH,
    WX_SE_FIELD_HEIGHT,

    WX_SE_SLIDER_RED,
    WX_SE_SLIDER_GREEN,
    WX_SE_SLIDER_BLUE,
    WX_SE_SLIDER_ALPHA,
    WX_SE_SLIDER_ZOOM,

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

static void floodFill(bmp_t* bmp, pixel_t src, pixel_t stat, unsigned int x, unsigned int y)
{
    pixel_t* p = (pixel_t*)px_at(bmp, x, y);
    if (!memcmp(p, &src, sizeof(pixel_t)) ||
        memcmp(p, &stat, sizeof(pixel_t))) {
        return;
    }
    memcpy(p, &src, sizeof(pixel_t));
    if (x + 1 < bmp->width) floodFill(bmp, src, stat, x + 1, y);
    if (x > 0) floodFill(bmp, src, stat, x - 1, y);
    if (y + 1 < bmp->height) floodFill(bmp, src, stat, x, y + 1);
    if (y > 0) floodFill(bmp, src, stat, x, y - 1);
}

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
    slider = group->widgets[WX_SE_SLIDER_ZOOM].widget;
    slider->lerp = clampf(0.0f, 1.0f, (scale - 0.5f) / SCALE_MULT);
}

static void wxInput()
{
    wxRect* rect = group->widgets[WX_SE_RECT_CURSOR].widget;
    rect->color = cursorColor;

    wxButton* button = group->widgets[WX_SE_BUTTON_MENU].widget;
    if (button->state == WIDGET_SELECTED) {
        systemSetState(STATE_MENU);
        return;
    }

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
    slider = group->widgets[WX_SE_SLIDER_ZOOM].widget;
    if (slider->selected) scale = 0.5f + slider->lerp * SCALE_MULT;

    if (mouse_down(GLFW_MOUSE_BUTTON_LEFT)) {
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

    if (window_file_has()) {
        bmp_t tmp = bmp_load(window_file_get());
        bmp_t tmp2 = bmp_resize_width(&tmp, 32);
        bmp_free(&tmp);
        tmp = bmp_resize_height(&tmp2, 32);
        bmp_free(&tmp2);
        bmp = bmp_flip_vertical(&tmp);
        bmp_free(&tmp);
    }

    bool mousePressed = mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    bool mouseDown = mouse_down(GLFW_MOUSE_BUTTON_LEFT);
    
    wxButton* button = group->widgets[WX_SE_BUTTON_NEW].widget;
    if (button->state == WIDGET_HOVER && mousePressed) {
        wxField* field = group->widgets[WX_SE_FIELD_WIDTH].widget;
        sscanf(field->text, "%u", &fieldWidth);
        fieldWidth = fieldWidth * (fieldWidth <= 128) + 128 * (fieldWidth > 128);
        sprintf(field->text, "%u", fieldWidth);

        field = group->widgets[WX_SE_FIELD_HEIGHT].widget;
        sscanf(field->text, "%u", &fieldHeight);
        fieldHeight = fieldHeight * (fieldHeight <= 128) + 128 * (fieldHeight > 128);
        sprintf(field->text, "%u", fieldHeight);

        bmp_free(&bmp);
        bmp = bmp_new(fieldWidth, fieldHeight, 4);
        memset(bmp.pixels, 255, bmp.width * bmp.height * bmp.channels);
    }
    wxField* field = group->widgets[WX_SE_FIELD_DIR].widget;
    button = group->widgets[WX_SE_BUTTON_LOAD].widget;
    if (button->state == WIDGET_HOVER && mousePressed) {
        bmp_t tmp = bmp_load(field->text);
        if (tmp.pixels != NULL) {
            bmp_free(&bmp);
            bmp = tmp;
        }
    }
    button = group->widgets[WX_SE_BUTTON_SAVE].widget;
    if (button->state == WIDGET_HOVER && mousePressed) {
        bmp_write(field->text, &bmp);
    }
    button = group->widgets[WX_SE_BUTTON_SUBMIT].widget;
    if (button->state == WIDGET_HOVER && mousePressed) {
        bmp_t tmp = bmp_flip_vertical(&bmp);
        spriteCollectionSubmitCustom(&tmp);
        bmp_free(&tmp);
        currentPlayerSprite = SPRITE_CUSTOM;
        *(unsigned int*)entity_get(player, COMPONENT_SPRITE_COLLECTION) = currentPlayerSprite;
    }
    
    wxGroupUpdate(group, mouse, mousePressed, mouseDown);

    if (mouseDown) {
        rect_t r = {
            0.5f * viewport.x / viewport.z, 
            0.5f * viewport.y / viewport.z, 
            (float)bmp.width * scale, 
            (float)bmp.height * scale
        };

        if (point_meeting(mouse, r)) {
            float dx = mouse.x - r.x + r.w * 0.5f;
            float dy = mouse.y - r.y + r.h * 0.5f;
            unsigned int x = (unsigned int)(int)clampf(dx / scale, 0.0f, bmp.width);
            unsigned int y = (unsigned int)(int)clampf(dy / scale, 0.0f, bmp.height);
            pixel_t p = ctop(cursorColor);
            uint8_t* ptr = px_at(&bmp, x, y);

            if (keyboard_down(GLFW_KEY_F)) floodFill(&bmp, p, *(pixel_t*)ptr, x, y);
            else memcpy(ptr, &p, sizeof(pixel_t));
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

void spriteEditorDirectoryReset()
{
    group = &wxDir.groups[WX_DIR_SPRITE_EDITOR];
}

void spriteEditorInit()
{
    spriteEditorDirectoryReset();
    uint8_t color[] = {255, 255, 255, 255};
    bmp = bmp_color(32, 32, 4, color);
    scale = 2.0f;
    cursorColor = unicolor(0.0f);
    cursorColor.a = 1.0f;
    slidersReset();
}