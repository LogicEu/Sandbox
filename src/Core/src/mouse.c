#include "common.h"

static double scroll_x = 0, scroll_y = 0;
static unsigned int mouse_state = GLFW_RELEASE;

/*
------------------
 -> Mouse Input <- 
------------------
*/

void GLmouse_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
    scroll_x = xoffset;
    scroll_y = yoffset;
}

void mouse_get_position(float* x, float* y)
{
    double dx, dy;
    glfwGetCursorPos(core.window, &dx, &dy);
    *x = (float)dx / core.scale;
    *y = (core.height - (float)dy) / core.scale;
}

void mouse_get_position3d(float* x, float* y)
{
    double dx = (double)(*x), dy = (double)(*y);
    glfwGetCursorPos(core.window, &dx, &dy);
    *x = (core.width * 0.5f - (float)dx);
    *y = (core.height * 0.5f - (float)dy);
}

void mouse_get_scroll(float* x, float* y)
{
    *x = scroll_x;
    *y = scroll_y;
}

unsigned int mouse_down(unsigned int button)
{
    return glfwGetMouseButton(core.window, button);
}

unsigned int mouse_pressed(unsigned int button)
{
    unsigned int ret = 0;
    int mouse_button = glfwGetMouseButton(core.window, button);
    if (mouse_button == GLFW_PRESS && mouse_state == GLFW_RELEASE) ret = 1;
    mouse_state = mouse_button;
    return ret;
}

unsigned int mouse_released(unsigned int button)
{
    return glfwGetMouseButton(core.window, button) == 0;
}

void mouse_set_mode(unsigned int mouse_lock)
{
    if (mouse_lock) glfwSetInputMode(core.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else glfwSetInputMode(core.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}