#include "UIcommon.h"

extern vec2 mouse;
extern wxDirectory wxDir;
static wxGroup* group;

typedef enum {
    WX_OPTIONS_TITLE_MAIN,
    WX_OPTIONS_TITLE_MOUSE_LOCK,
    WX_OPTIONS_TITLE_FULLSCREEN,

    WX_OPTIONS_MOUSE_LOCK,
    WX_OPTIONS_FULLSCREEN
} wxOptionEnum;

static bool mouseLock = false;

static void getInput()
{
    if (keyboard_pressed(GLFW_KEY_ESCAPE)) {
        systemSetState(STATE_MENU);
    }

    bool mousePressed = mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    wxGroupUpdate(group, mouse, mousePressed, mousePressed);

    wxSwitch* sw = group->widgets[WX_OPTIONS_MOUSE_LOCK].widget;
    if (!mouseLock && sw->activated) {
        mouseLock = true;
        mouse_set_mode(mouseLock);
    }
    if (mouseLock && !sw->activated) {
        mouseLock = false;
        mouse_set_mode(mouseLock);
    }
}

static void optionsDraw()
{
    wxGroupDraw(group);
    mouseDraw();
}

void optionsStep()
{
    getInput();
    optionsDraw();
}

void optionsInit()
{
    group = &wxDir.groups[WX_DIR_OPTIONS];
}