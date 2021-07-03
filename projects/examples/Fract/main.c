#include <stdio.h>
#include <Fract/Fract.h>

int main(void)
{
    int size = 100;
    float a = -1.0f;
    float b = 1.0f;
    float f = size / 2 - size;
    for (int i = 0; i < size; i++) {
        float v = (f + i) / 100.0f;
        printf("%f %f\n", v, smoothstep(a, b, v));
    }
    return 0;
}
