#include "../game.h"
#include <stdlib.h>
#include <string.h>

pixel_t* pixel_at(bitmap_t* bitmap, unsigned int x, unsigned int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

bitmap_t* bitmap_new(unsigned int width, unsigned int height)
{
    bitmap_t* bitmap = (bitmap_t*)malloc(sizeof(bitmap_t));
    bitmap->pixels = (pixel_t*)malloc(width * height * sizeof(pixel_t));
    bitmap->height = height;
    bitmap->width = width;
    return bitmap;
}

bitmap_t* bitmap_color(unsigned int width, unsigned int height, pixel_t pixel)
{
    bitmap_t* bitmap = bitmap_new(width, height);
    for (int y = 0; y < height; y ++) {
        for (int x = 0; x < width; x ++) {
            *pixel_at(bitmap, x, y) = pixel;
        }
    }
    return bitmap;
}

bitmap_t* bitmap_load(const char* path) 
{
    bitmap_t* bitmap = (bitmap_t*)malloc(sizeof(bitmap_t));
    bitmap->pixels = (pixel_t*)png_file_load(path, &bitmap->width, &bitmap->height);
    return bitmap;
}

void bitmap_free(bitmap_t* bitmap)
{
    if (bitmap->pixels != NULL) {
        free(bitmap->pixels);
    }
    free(bitmap);
}

bitmap_t* bitmap_mirror_horizontal(bitmap_t* bitmap)
{
    bitmap_t* ret = bitmap_new(bitmap->width, bitmap->height);
    for (int y = 0; y < ret->height; y++) {
        for (int x = 0; x < ret->width; x++) {
            memcpy(pixel_at(ret, x, y), pixel_at(bitmap, bitmap->width - x - 1, y), sizeof(pixel_t));
        }
    }
    bitmap_free(bitmap);
    return ret;
}

bitmap_t* bitmap_mirror_vertical(bitmap_t* bitmap)
{
    bitmap_t* ret = bitmap_new(bitmap->width, bitmap->height);
    for (int y = 0; y < ret->height; y++) {
        for (int x = 0; x < ret->width; x++) {
            memcpy(pixel_at(ret, x, y), pixel_at(bitmap, x, bitmap->height - y - 1), sizeof(pixel_t));
        }
    }
    bitmap_free(bitmap);
    return ret;
}