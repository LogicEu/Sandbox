#include <Core/Core.h>

/*
-----------------
 -> Time Input <- 
-----------------
*/

float time_start_timer()
{
    return (float)glfwGetTime();
}

float time_end_timer(float start)
{
    return (float)glfwGetTime() - start;
}

float time_get_delta_time(float* last_time)
{
    float current_time = glfwGetTime();
    float delta_time = current_time - *last_time;
    *last_time = current_time;
    return delta_time;
}