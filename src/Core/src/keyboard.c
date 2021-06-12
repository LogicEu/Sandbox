#include "common.h"

/*
---------------------
 -> Keyboard Input <- 
---------------------
*/

static unsigned int keys[1024], pressedKeys[1024], char_ret = 1;
static char key_char = '\0';

void GLkeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key < 128 && !keys[key]) {
        if (mods == GLFW_MOD_CAPS_LOCK || mods == GLFW_MOD_SHIFT || key < 65) key_char = key;
        else key_char = key + 32;
        char_ret = 1;
    } 
    
    keys[key] = action;
    pressedKeys[key] = pressedKeys[key] * (action != 0);
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

char keyboard_char()
{
    char ret = '\0';
    if (char_ret) {
        ret = key_char;
        char_ret = 0;
    }
    return ret;
}