#include "common.h"
#include <stdio.h>

/*
-----------
 -> Core <- 
-----------
*/

CoreManager core;

void coreInit()
{
    if (!glfwInit()) {
        printf("GLFW Library could not initiate.\n");
        return;
    }
    core.window = NULL;
    core.width = 800;
    core.height = 600;
    core.scale = 1;
    core.fullscreen = 0;
    core.mode = 0;
    core.mouse_lock = 0;
    core.update = 0;
}

void coreExit()
{
    glfwTerminate();
}

void core_set_2d_scale(unsigned int scale)
{
    core.scale = scale + (scale == 0);
}

unsigned int core_get_2d_scale()
{
    return core.scale;
}

void core_get_size(unsigned int* width, unsigned int* height)
{
    *width = core.width;
    *height = core.height;
}

void core_set_3d_mode()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    core.mode = 1;
}

void core_set_2d_mode()
{
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    core.mode = 0;
}

unsigned int core_need_update()
{
    return core.update;
}

void core_update()
{
    core.update = 0;
}
