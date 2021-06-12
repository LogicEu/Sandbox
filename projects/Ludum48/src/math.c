#include "../game.h"

float absf(float f)
{
    return (f >= 0.0f) * f + (f < 0.0f) * -f;
}

float clampf(float val, float min, float max)
{
    float t = (val < min) * min + (val >= min) * val;
    return (t > max) * max + (t <= max) * t;
}

float lerpf(float a, float b, float t)
{
    return (a * (1.0f - t)) + (b * t);
}