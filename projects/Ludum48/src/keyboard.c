#include "../game.h"

static unsigned int keys[1024], pressedKeys[1024];

void GLkeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    keys[key] = action;
    pressedKeys[key] = pressedKeys[key] && action != 0;
}

unsigned int keyboard_down(unsigned int k)
{
    return keys[k];
}

unsigned int keyboard_pressed(unsigned int k)
{
    if (!pressedKeys[k] && keys[k]) {
        pressedKeys[k] = 1;
        return 1;
    }
    return 0;
}

unsigned int keyboard_released(unsigned int k)
{
    if (!keys[k] && pressedKeys[k]) {
        pressedKeys[k] = 0;
        return 1;
    }
    return 0;
}