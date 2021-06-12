#include <Core/Core.h>

typedef struct CoreManager {
    GLFWwindow* window;
    unsigned int width, height, scale, fullscreen, mode, mouse_lock, update;
} CoreManager;

extern CoreManager core;

void GLmouse_scroll(GLFWwindow* window, double xoffset, double yoffset);
void GLkeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void GLdrop_file(GLFWwindow* windwo, int count, const char** paths);