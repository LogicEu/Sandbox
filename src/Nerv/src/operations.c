#include "../Nerv.h"
#include <stdlib.h>
#include <math.h>

float utof(uint8_t u)
{
    return ((float)(int)u) / 255.0f;
}

float sigmoid(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

float dsigmoid(float x)
{
    double s = sigmoid(x);
    return s * (1.0f - s);
}

float relu(float x)
{
    return x * (x > 0.0f);
}

float drelu(float x)
{
    return (float)(x > 0.0f);
}

float rand_weight()
{
    return (float)rand() / (float)RAND_MAX;
}