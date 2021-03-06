#include <Imagine/Imagine.h>

static float _lerpf(float a, float b, float t)
{
    return (a * (1.0 - t)) + (b * t);
}

static float _inverse_lerpf(float a, float b, float val)
{
    return (val - a) / (b - a);
}

static float _remapf(float input_a, float input_b, float out_a, float out_b, float val)
{
    float t = _inverse_lerpf(input_a, input_b, val);
    return _lerpf(out_a, out_b, t);
}

static uint8_t ulerp(uint8_t c1, uint8_t c2, float f)
{
    float f1 = (float)(int)(unsigned int)c1;
    float f2 = (float)(int)(unsigned int)c2;
    return (uint8_t)(unsigned int)(int)_lerpf(f1, f2, f);
}

static void pxlerp(unsigned char* p1, unsigned char* p2, float f, unsigned int channels, unsigned char* out)
{
    for (unsigned int i = 0; i < channels; i++) {
        out[i] = ulerp(p1[i], p2[i], f);
    }
}

static void bmp_min_max(bmp_t* bitmap, unsigned* x_min, unsigned* y_min, unsigned* x_max, unsigned* y_max)
{
    unsigned int min_x, min_y, max_x, max_y;
    min_x = min_y = 100000;
    max_x = max_y = 0;

    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            min_x = min_x * (x >= min_x) + x * (x < min_x);
            min_y = min_y * (y >= min_y) + y * (y < min_y);
            max_x = max_x * (x <= max_x) + x * (x > max_x);
            max_y = max_y * (y <= max_y) + y * (y > max_y);
        }
    }

    *x_min = min_x;
    *y_min = min_y;
    *x_max = max_x;
    *y_max = max_y;
}

bmp_t bmp_cut(bmp_t* bitmap)
{
    unsigned int x_min, y_min, x_max, y_max;
    bmp_min_max(bitmap, &x_min, &y_min, &x_max, &y_max);

    bmp_t new_bitmap = bmp_new(x_max - x_min + 1, y_max - y_min + 1, bitmap->channels);
    for (unsigned int y = 0; y < new_bitmap.height; y++) {
        for (unsigned int x = 0; x < new_bitmap.width; x++) {
            memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, x + x_min, y + y_min), bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_flip_horizontal(bmp_t* bitmap) 
{
    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, bitmap->channels);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, bitmap->width - 1 - x, y), bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_flip_vertical(bmp_t* bitmap)
{
    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, bitmap->channels);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, x, bitmap->height - 1 - y), bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_greyscale(bmp_t* bitmap)
{
    printf("Greyscale transformation\n");
    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, 1);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            px_t p = px_at(bitmap, x, y);
            int m = 0;
            int div = bitmap->channels * (bitmap->channels <= 3) + 3 * (bitmap->channels > 3);
            for (int j = 0; j < div; j++) {
                m += (int)p[j];
            }
            m /= div;
            memset(px_at(&new_bitmap, x, y), m, 1);
        }
    }
    return new_bitmap;
}

bmp_t bmp_black_and_white(bmp_t* bitmap) 
{
    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, bitmap->channels);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            px_t p = px_at(bitmap, x, y);
            int m = 0;
            for (unsigned int j = 0; j < bitmap->channels; j++) {
                m += (unsigned int)p[j];
            }
            m /= bitmap->channels;
            memset(px_at(&new_bitmap, x, y), m, bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_rotate(bmp_t* bitmap)
{
    bmp_t new_bitmap = bmp_new(bitmap->height, bitmap->width, bitmap->channels);
    for (unsigned int y = 0; y < new_bitmap.height; y++) {
        for (unsigned int x = 0; x < new_bitmap.width; x++) {
            memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, y, x), bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_scale(bmp_t* bitmap)
{
    bmp_t new_bitmap = bmp_new(bitmap->width * 2, bitmap->height * 2, bitmap->channels);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            unsigned int xx = x * 2, yy = y * 2;
            px_t p = px_at(bitmap, x, y);
            memcpy(px_at(&new_bitmap, xx, yy), p, bitmap->channels);
            memcpy(px_at(&new_bitmap, xx + 1, yy), p, bitmap->channels);
            memcpy(px_at(&new_bitmap, xx, yy + 1), p, bitmap->channels);
            memcpy(px_at(&new_bitmap, xx + 1, yy + 1), p, bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_white_to_transparent(bmp_t* bitmap)
{
    static pixel_t white = {255, 255, 255, 255};
    static pixel_t transparent = {0, 0, 0, 0};

    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, 4);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            if (!memcmp(&white, px_at(bitmap, x, y), bitmap->channels)) {
                memcpy(px_at(&new_bitmap, x, y), &transparent, new_bitmap.channels);
            } else {
                memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, x, y), bitmap->channels);
                *(px_at(&new_bitmap, x, y) + 3) = 255;
            }
        }
    }
    return new_bitmap;
}

bmp_t bmp_clear_to_transparent(bmp_t* bitmap, uint8_t sensibility)
{
    static pixel_t transparent = {0, 0, 0, 0};

    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, 4);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            unsigned int t = 1;
            for (unsigned int z = 0; z < bitmap->channels; z++) {
                if (*(px_at(bitmap, x, y) + z) <= sensibility) {
                    t = 0;
                    break;
                }
            }
            if (t) memcpy(px_at(&new_bitmap, x, y), &transparent, new_bitmap.channels);
            else {
                memcpy(px_at(&new_bitmap, x, y), px_at(bitmap, x, y), bitmap->channels);
                *(px_at(&new_bitmap, x, y) + 3) = 255;
            }
        }
    }
    return new_bitmap;
}

static void pxaverage(px_t* in, px_t out, unsigned int channels)
{
    unsigned int temp[channels];
    for (unsigned int i = 0; i < channels; i++) {
        temp[i] = 0;
        for (unsigned int j = 0; j < 4; j++) {
            temp[i] += (unsigned int)in[j][i];
        }
        out[i] = (uint8_t)(temp[i] / 4);
    }
}

bmp_t bmp_reduce(bmp_t* bitmap)
{
    bmp_t new_bitmap = bmp_new(bitmap->width / 2, bitmap->height / 2, bitmap->channels);
    for (unsigned int y = 0; y < new_bitmap.height; y++) {
        for (unsigned int x = 0; x < new_bitmap.width; x++) {
            px_t p[4];
            uint8_t cpy[bitmap->channels];
            p[0] = px_at(bitmap, x * 2, y * 2);
            p[1] = px_at(bitmap, x * 2 + 1, y * 2);
            p[2] = px_at(bitmap, x * 2, y * 2 + 1);
            p[3] = px_at(bitmap, x * 2 + 1, y * 2 + 1);
            pxaverage(&p[0], &cpy[0], bitmap->channels);
            memcpy(px_at(&new_bitmap, x, y), &cpy[0], bitmap->channels);
        }
    }
    return new_bitmap;
}

bmp_t bmp_jcompress(bmp_t* bitmap, unsigned int quality)
{
    if (quality > 100) quality = 100;
    bmp_t b;
    b.width = bitmap->width;
    b.height = bitmap->height;
    b.channels = 3;
    b.pixels = img_jcompress(bitmap->pixels, bitmap->width, bitmap->height, bitmap->channels, quality);
    return b;
}

bmp_t bmp_transform(bmp_t* bitmap, unsigned int channels)
{
    bmp_t ret;
    ret.width = bitmap->width;
    ret.height = bitmap->height;
    ret.channels = channels;
    ret.pixels = img_transform_buffer(bitmap->pixels, bitmap->width, bitmap->height, bitmap->channels, channels);
    return ret;
}

bmp_t bmp_negative(bmp_t* bitmap)
{
    bmp_t new_bitmap = bmp_new(bitmap->width, bitmap->height, bitmap->channels);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            for (unsigned int z = 0; z < bitmap->channels; z++) {
                *(px_at(&new_bitmap, x, y) + z) = 255 - *(px_at(bitmap, x, y) + z);
            }
        }
    }
    return new_bitmap;
}

bmp_t bmp_resize_width(bmp_t* bmp, unsigned int target_width)
{
    bmp_t new_bmp = bmp_new(target_width, bmp->height, bmp->channels);
    for (unsigned int y = 0; y < bmp->height; y++) {
        for (unsigned int x = 0; x < target_width; x++) {
            float dx = _remapf(0.0f, (float)target_width, 0.0f, (float)bmp->width, (float)x);
            float dif = dx - (float)((unsigned int)dx);
            unsigned int xx = dx - dif;

            if (xx + 1 < target_width) {
                unsigned char out[bmp->channels];
                pxlerp(px_at(bmp, xx, y), px_at(bmp, xx + 1, y), dif, bmp->channels, out);
                memcpy(px_at(&new_bmp, x, y), out, bmp->channels);
            }
            else memcpy(px_at(&new_bmp, x, y), px_at(bmp, xx, y), bmp->channels);
        }
    }
    return new_bmp;
}

bmp_t bmp_resize_height(bmp_t* bmp, unsigned int target_height)
{
    bmp_t new_bmp = bmp_new(bmp->width, target_height, bmp->channels);
    for (unsigned int y = 0; y < target_height; y++) {
        for (unsigned int x = 0; x < bmp->width; x++) {
            float dy = _remapf(0.0f, (float)target_height, 0.0f, (float)bmp->height, (float)y);
            float dif = dy - (float)((unsigned int)dy);
            unsigned int yy = dy - dif;

            if (yy + 1 < target_height) {
                unsigned char out[bmp->channels];
                pxlerp(px_at(bmp, x, yy), px_at(bmp, x, yy + 1), dif, bmp->channels, out);
                memcpy(px_at(&new_bmp, x, y), out, bmp->channels);
            }
            else memcpy(px_at(&new_bmp, x, y), px_at(bmp, x, yy), bmp->channels);
        }
    }
    return new_bmp;
}

bmp_t bmp_scale_lerp(bmp_t* bmp, float f)
{
    unsigned int target_width = (unsigned int)((float)bmp->width * f);
    unsigned int target_height = (unsigned int)((float)bmp->height * f);
    bmp_t temp = bmp_resize_width(bmp, target_width);
    bmp_t ret = bmp_resize_height(&temp, target_height);
    bmp_free(&temp);
    return ret;
}
