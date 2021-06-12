#ifndef IMAGINE_IMAGE_H
#define IMAGINE_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*

=======================================================

>>>>>>>>>>>>>>  IMAGINE IMAGE LIBRARY   >>>>>>>>>>>>>>>

ANSI C99 header only framework for easy managing I/O PNG
image file format. Built on top of libpng.

=============================================  @eulogic

*/

//#include <Omega.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <libpng/png.h>

/*
--------------------------
 -> Graphic Data Types  <- 
--------------------------
*/

typedef enum {
    IMG_NULL,
    IMG_BW,
    IMG_BWA,
    IMG_RGB,
    IMG_RGBA
} img_channels_enum;

typedef enum {
    IMG_FORMAT_NULL,
    IMG_FORMAT_PNG,
    IMG_FORMAT_JPG,
    IMG_FORMAT_PPM,
    IMG_FORMAT_GIF
} img_format_enum;

typedef struct {
    uint8_t* pixels;
    unsigned int width, height, channels;
} bmp_t;

typedef uint8_t* px_t;
typedef uint8_t px1;

typedef struct px2 {
    uint8_t g, a;
} px2;

typedef struct px3 {
    uint8_t r, g, b;
} px3;

typedef struct px4 {
    uint8_t r, g, b, a;
} px4;

typedef px4 pixel_t;
typedef px3 rgb_t;

typedef struct {
    pixel_t* pixels;
    unsigned int width, height;
} bitmap_t; // 16bytes

typedef struct {
    float r, g, b, a;
} color_t; //16 bytes

typedef struct {
    rgb_t** frames;
    rgb_t background;
    unsigned int size, used, width, height;
} gif_t;

/*
--------------------------
 -> IMG Core Functions  <- 
--------------------------
*/

unsigned char* img_file_load(const char* path, unsigned int* width, unsigned int* height, unsigned int* out_channels);
void img_file_write(const char* path, unsigned char* img, unsigned int width, unsigned int height, unsigned int in_channels);
unsigned char* img_jcompress(unsigned char* img, unsigned int width, unsigned int height, unsigned int channels, unsigned int quality);
void img_set_jpeg_quality(int quality);
unsigned char* img_transform_buffer(unsigned char* buffer, unsigned int width, unsigned int height, unsigned int src, unsigned int dest);

/*
--------------------------
 -> Basic PNG File I/O  <- 
--------------------------
*/

unsigned char* png_file_load(const char* path, unsigned int* width, unsigned int* height);
void png_file_write(const char* path, unsigned char* data, unsigned int width, unsigned int height);

/*
---------------------------
 -> Basic JPEG File I/O  <- 
---------------------------
*/

unsigned char* jpeg_file_load(const char* path, unsigned int* w, unsigned int* h);
void jpeg_file_write(const char* path, unsigned char* data, unsigned int width, unsigned int height, int quality);
unsigned char* jpeg_compress(unsigned char* data, size_t* size, unsigned int width, unsigned height, int quality);
unsigned char* jpeg_decompress(unsigned char* data, size_t size);

/*
--------------------------
 -> Basic PPM File I/O  <- 
--------------------------
*/

unsigned char* ppm_file_load(const char* path, unsigned int* width, unsigned int* height);
void ppm_file_write(const char* path, unsigned char* img, unsigned int width, unsigned int height);

/*
--------------------------
 -> Basic PPM File I/O  <- 
--------------------------
*/

gif_t* gif_file_load(const char* path);
unsigned char* gif_file_load_frame(const char* path, unsigned int* width, unsigned int* height);
void gif_free(gif_t* gif);
void gif_file_write(const char* path, gif_t* input);
void gif_file_write_frame(const char* path, unsigned char* img, unsigned int width, unsigned int height);

bmp_t* gif_to_bmp(gif_t* gif, unsigned int* count);
gif_t* bmp_to_gif(bmp_t* bitmaps, unsigned int count);

/*
------------------------------
 -> RGB / RGBA Conversions  <- 
------------------------------
*/

unsigned char* rgb_to_rgba(unsigned char* buffer, unsigned int width, unsigned int height);
unsigned char* rgba_to_rgb(unsigned char* buffer, unsigned int width, unsigned int height);

/*
-----------------------------------------
 -> Pixel RGBA uint8_t Data Structure  <- 
-----------------------------------------
***32bit***
*/

pixel_t pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
pixel_t unipixel(uint8_t i);
int pixel_cmp(pixel_t p1, pixel_t p2);

/*
-----------------------------------------
 -> Pixel RGB uint8_t Data Structure  <- 
-----------------------------------------
***24bit***
*/

rgb_t rgb(uint8_t r, uint8_t g, uint8_t b);
rgb_t unirgb(uint8_t i);
int rgbcmp(rgb_t p1, rgb_t p2);

/*
-----------------------------
 -> Bitmap Data Structure  <- 
-----------------------------
***32bit***
*/

pixel_t* pixel_at(bitmap_t* bitmap, unsigned int x, unsigned int y);
bitmap_t* bitmap_new(unsigned int width, unsigned int height);
bitmap_t* bitmap_color(unsigned int width, unsigned int height, pixel_t pixel);
bitmap_t* bitmap_load(const char* path);
void bitmap_write(const char* path, bitmap_t* bitmap);
void bitmap_free(bitmap_t* bitmap);

bitmap_t* bitmap_flip_vertical(bitmap_t* bitmap);
bitmap_t* bitmap_flip_horizontal(bitmap_t* bitmap);
bitmap_t* bitmap_black_and_white(bitmap_t* bitmap);
bitmap_t* bitmap_rotate(bitmap_t* bitmap);
bitmap_t* bitmap_scale(bitmap_t* bitmap);
bitmap_t* bitmap_white_to_transparent(bitmap_t* bitmap);
bitmap_t* bitmap_cut(bitmap_t* bitmap);
bitmap_t* bitmap_reduce(bitmap_t* bitmap);

/*
-----------------------------
 -> Bitmap Data Structure  <- 
-----------------------------
***24bit***
*/

unsigned char* px_at(bmp_t* bitmap, unsigned int x, unsigned int y);
bmp_t bmp_new(unsigned int width, unsigned int height, unsigned int channels);
bmp_t bmp_color(unsigned int width, unsigned int height, unsigned int channels, uint8_t* color);
bmp_t bmp_load(const char* path);
void bmp_write(const char* path, bmp_t* bitmap);
void bmp_free(bmp_t* bitmap);

bmp_t bmp_negative(bmp_t* bitmap);
bmp_t bmp_flip_vertical(bmp_t* bitmap);
bmp_t bmp_flip_horizontal(bmp_t* bitmap);
bmp_t bmp_black_and_white(bmp_t* bitmap);
bmp_t bmp_rotate(bmp_t* bitmap);
bmp_t bmp_scale(bmp_t* bitmap);
bmp_t bmp_white_to_transparent(bmp_t* bitmap);
bmp_t bmp_cut(bmp_t* bitmap);
bmp_t bmp_reduce(bmp_t* bitmap);
bmp_t bmp_clear_to_transparent(bmp_t* bitmap, uint8_t sensibility);
bmp_t bmp_transform(bmp_t* bitmap, unsigned int channels);
bmp_t bmp_jcompress(bmp_t* bitmap, unsigned int quality);

bmp_t bmp_resize_width(bmp_t* bmp, unsigned int target_width);
bmp_t bmp_resize_height(bmp_t* bmp, unsigned int target_height);
bmp_t bmp_scale_lerp(bmp_t* bmp, float f);


/*
---------------------------------------
 -> Color RGBA Float Data Structure  <- 
--------------------------------------
*/

color_t color(float r, float g, float b, float a);
color_t unicolor(float f);
color_t color_mult(color_t color, float f);
int color_cmp(color_t c1, color_t c2);
color_t ptoc(pixel_t p);
pixel_t ctop(color_t c);


#ifdef __cplusplus
}
#endif
#endif
