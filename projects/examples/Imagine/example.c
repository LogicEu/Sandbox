#include <Imagine/Imagine.h>

float lerpf(float a, float b, float t)
{
    return (a * (1.0 - t)) + (b * t);
}

float inverse_lerpf(float a, float b, float val)
{
    return (val - a) / (b - a);
}

float remapf(float input_a, float input_b, float out_a, float out_b, float val)
{
    float t = inverse_lerpf(input_a, input_b, val);
    return lerpf(out_a, out_b, t);
}

static uint8_t ulerp(uint8_t c1, uint8_t c2, float f)
{
    float f1 = (float)(int)(unsigned int)c1;
    float f2 = (float)(int)(unsigned int)c2;
    return (uint8_t)(unsigned int)(int)lerpf(f1, f2, f);
}

static void px_lerp(unsigned char* p1, unsigned char* p2, float f, unsigned int channels, unsigned char* out)
{
    for (unsigned int i = 0; i < channels; i++) {
        out[i] = ulerp(p1[i], p2[i], f);
    }
}

int main(int argc, char** argv)
{
    char input_path[128], output_path[128] = "output.png";
    if (argc < 2) {
        printf("$1 - Input png image\n$2 - Output png image\n");
        return 0;
    } else {
        if (argc > 2) strcpy(output_path, argv[2]);
        strcpy(input_path, argv[1]);
    }

    bmp_t bitmap = bmp_load(input_path);
    bmp_t b = bmp_resize_width(&bitmap, 200);

    bmp_write(output_path, &b);
    bmp_free(&bitmap);
    bmp_free(&b);
    return 0;
}
