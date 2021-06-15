#include "Tree.h"

int main(void)
{
    unsigned int state = STATE_MENU;
    systemInit(state);
    float lastTime = time_start_timer(), deltaTime;
    while (window_is_open()) {
        deltaTime = time_get_delta_time(&lastTime);
        systemStep(deltaTime);
    }
    systemExit();
}
