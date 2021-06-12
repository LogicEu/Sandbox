#include "common.h"

/*
-------------
 -> Screen <- 
-------------
*/

void screen_color(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}

void screen_clear()
{   
    if (core.mode) glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    else glClear(GL_COLOR_BUFFER_BIT);
}

void screen_refresh()
{
    glfwSwapBuffers(core.window);
    glfwPollEvents();
}