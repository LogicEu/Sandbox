#include <Core/Core.h>
#include <stdio.h>

int main()
{
    coreInit();
    window_create("Core", 640, 480, 0, 0);
    screen_color(0.0f, 0.0f, 1.0f, 1.0f);
    while (!glfwWindowShouldClose(window_get())) {
        screen_clear();
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        screen_refresh();
    }

    coreExit();
    return 0;
}
