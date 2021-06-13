#include "UIcommon.h"

extern vec2 mouse;
extern vec4 cam;

wxGroup* group;

static void getInput()
{
    wxField* f = group->widgets[3].widget;
    wxField* ff = group->widgets[4].widget;
    if (keyboard_pressed(GLFW_KEY_ENTER)) {
        printf("%s:%s\n", ff->text, f->text);
    }
    if (keyboard_pressed(GLFW_KEY_ESCAPE)) {
        systemSetState(STATE_MENU);
    }

    bool mousePressed = mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    wxGroupUpdate(group, mouse, mousePressed, mousePressed);
}

static void netMenuDraw()
{
    wxGroupDraw(group);
    mouseDraw();
}

void netMenuStep()
{
    getInput();
    netMenuDraw();
    cameraBackgroundSlide();
}

void netMenuInit()
{
    group = &wxDir.groups[WX_DIR_NET_MENU];
}