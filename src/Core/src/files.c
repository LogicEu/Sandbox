#include <Core/Core.h>
#include <string.h>

static unsigned new_file = 0;
static unsigned int used = 0;
static char file_paths[16][256];

void GLdrop_file(GLFWwindow* window, int count, const char** paths)
{
    if (used >= 16) return;
    for (int i = 0; i < count; i++) {
        strcpy(file_paths[used++], paths[i]);
        if (used == 15) break;
    }
    new_file = 1;
}

unsigned int window_file_has()
{
    return used;
}

char* window_file_get()
{
    if (used) return file_paths[--used];
    return NULL;
}
