#include <Imagine/Imagine.h>

static void bitmap_min_max(bitmap_t* bitmap, unsigned* x_min, unsigned* y_min, unsigned* x_max, unsigned* y_max)
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

bitmap_t* bitmap_cut(bitmap_t* bitmap)
{
    unsigned int x_min, y_min, x_max, y_max;
    bitmap_min_max(bitmap, &x_min, &y_min, &x_max, &y_max);

    bitmap_t* new_bitmap = bitmap_new(x_max - x_min + 1, y_max - y_min + 1);
    for (unsigned int y = 0; y < new_bitmap->height; y++) {
        for (unsigned int x = 0; x < new_bitmap->width; x++) {
            *pixel_at(new_bitmap, x, y) = *pixel_at(bitmap, x + x_min, y + y_min);
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_flip_horizontal(bitmap_t* bitmap) 
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->width, bitmap->height);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            *pixel_at(new_bitmap, x, y) = *pixel_at(bitmap, bitmap->width - 1 - x, y);
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_flip_vertical(bitmap_t* bitmap)
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->width, bitmap->height);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            *pixel_at(new_bitmap, x, y) = *pixel_at(bitmap, x, bitmap->height - 1 - y);
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_black_and_white(bitmap_t* bitmap) 
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->width, bitmap->height);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            pixel_t p = *pixel_at(bitmap, x, y);
            unsigned int m =  (p.r + p.g + p.b) / 3;
            pixel_t ret = {m, m, m, p.a};
            *pixel_at(new_bitmap, x, y) = ret;
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_rotate(bitmap_t* bitmap)
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->height, bitmap->width);
    for (unsigned int y = 0; y < new_bitmap->height; y++) {
        for (unsigned int x = 0; x < new_bitmap->width; x++) {
            *pixel_at(new_bitmap, x, y) = *pixel_at(bitmap, y, x);
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_scale(bitmap_t* bitmap)
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->width * 2, bitmap->height * 2);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            unsigned int xx = x * 2, yy = y * 2;
            pixel_t p = *pixel_at(bitmap, x, y);
            *pixel_at(new_bitmap, xx, yy) = p;
            *pixel_at(new_bitmap, xx + 1, yy) = p;
            *pixel_at(new_bitmap, xx + 1, yy + 1) = p;
            *pixel_at(new_bitmap, xx, yy + 1) = p;
        }
    }
    return new_bitmap;
}

bitmap_t* bitmap_white_to_transparent(bitmap_t* bitmap)
{
    static pixel_t white = {255, 255, 255, 255};
    static pixel_t transparent = {0, 0, 0, 0};

    bitmap_t* new_bitmap = bitmap_new(bitmap->width, bitmap->height);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            if (!memcmp(&white, pixel_at(bitmap, x, y), sizeof(pixel_t))) {
                *pixel_at(new_bitmap, x, y) = transparent;
            } else *pixel_at(new_bitmap, x, y) = *pixel_at(bitmap, x, y);
        }
    }
    return new_bitmap;
}

static pixel_t pixaverage(pixel_t* p)
{
    unsigned int r = (unsigned int)p[0].r + (unsigned int)p[1].r + (unsigned int)p[2].r + (unsigned int)p[3].r;
    unsigned int g = (unsigned int)p[0].g + (unsigned int)p[1].g + (unsigned int)p[2].g + (unsigned int)p[3].g;
    unsigned int b = (unsigned int)p[0].b + (unsigned int)p[1].b + (unsigned int)p[2].b + (unsigned int)p[3].b;
    unsigned int a = (unsigned int)p[0].a + (unsigned int)p[1].a + (unsigned int)p[2].a + (unsigned int)p[3].a;
    pixel_t ret = {(uint8_t)(r / 4), (uint8_t)(g / 4), (uint8_t)(b / 4), (uint8_t)(a / 4)};
    return ret;
}

bitmap_t* bitmap_reduce(bitmap_t* bitmap)
{
    bitmap_t* new_bitmap = bitmap_new(bitmap->width / 2, bitmap->height / 2);
    for (unsigned int y = 0; y < new_bitmap->height; y++) {
        for (unsigned int x = 0; x < new_bitmap->width; x++) {
            pixel_t p[4];
            p[0] = *pixel_at(bitmap, x * 2, y * 2);
            p[1] = *pixel_at(bitmap, x * 2 + 1, y * 2);
            p[2] = *pixel_at(bitmap, x * 2, y * 2 + 1);
            p[3] = *pixel_at(bitmap, x * 2 + 1, y * 2 + 1);
            *pixel_at(new_bitmap, x, y) = pixaverage(&p[0]);
        }
    }
    return new_bitmap;
}
