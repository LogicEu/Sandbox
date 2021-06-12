#include "Tree.h"

int main(void)
{
    unsigned int state = STATE_MENU;
    // if (argc > 1) state = atoi(argv[1]);
    systemInit(state);
    float lastTime = time_start_timer(), deltaTime;
    while (window_is_open()) {
        deltaTime = time_get_delta_time(&lastTime);
        systemStep(deltaTime);
    }
    systemExit();
}
