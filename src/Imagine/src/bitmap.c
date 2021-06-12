#include <Imagine/Imagine.h>

/*
-----------------------------
 -> Bitmap Data Structure  <- 
-----------------------------
*/

extern unsigned char* img_transform_buffer(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int src, unsigned int dest);

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
    for (unsigned int y = 0; y < height; y ++) {
        for (unsigned int x = 0; x < width; x ++) {
            *pixel_at(bitmap, x, y) = pixel;
        }
    }
    return bitmap;
}

bitmap_t* bitmap_load(const char* path) 
{
    bitmap_t* bitmap = (bitmap_t*)malloc(sizeof(bitmap_t));
    unsigned int channels;
    unsigned char* buffer = img_file_load(path, &bitmap->width, &bitmap->height, &channels);
    if (channels != IMG_RGBA) {
        bitmap->pixels = (pixel_t*)img_transform_buffer(buffer, bitmap->width, bitmap->height, channels, IMG_RGBA);
        free(buffer);
    } else bitmap->pixels = (pixel_t*)buffer;
    return bitmap;
}

void bitmap_write(const char* path, bitmap_t* bitmap) 
{
    img_file_write(path, (unsigned char*)bitmap->pixels, bitmap->width, bitmap->height, IMG_RGBA);
}

void bitmap_free(bitmap_t* bitmap)
{
    if (bitmap->pixels != NULL) {
        free(bitmap->pixels);
    }
    free(bitmap);
}