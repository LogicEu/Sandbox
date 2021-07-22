#include "../Matrix.h"
#include <stdlib.h>
#include <math.h>

float normalize(float x)
{
    return x * (0.0f < x < 1.0f) + (float)(x >= 1.0f);
}

uint8_t ftou8(float normalized)
{
    return (int)(normalized / 255.0f);
}

float u8tof(uint8_t u)
{
    return ((float)(int)u) / 255.0f;
}

float sigmoid(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

float sigderiv(float sig)
{
    return sig * (1.0f - sig);
}

float dsigmoid(float x)
{
    return sigderiv(sigmoid(x));
}

float relu(float x)
{
    return x * (x > 0.0f);
}

float drelu(float x)
{
    return (float)(x > 0.0f);
}

float rand_normal()
{
    return (float)rand() / (float)RAND_MAX;
}