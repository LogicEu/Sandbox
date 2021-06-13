#ifndef TAXI_TEXTURES_H
#define TAXI_TEXTURES_H

#ifdef __cplusplus
extern "C" {
#endif

/*

=====================================================

>>>>>>>>>>>>>>  TAXI TEXTURES FRAME   >>>>>>>>>>>>>>>>

ANSI C99 header only texture handler for OpenGL.

============================================  @eulogic

*/

#include <Omega.h>
#include <Imagine/Imagine.h>
#include <Core/Core.h>

typedef struct {
    unsigned int id, width, height;
} texture_t; 

typedef struct {
    unsigned int VAO;
    texture_t cubemap;
} skybox_t;

typedef struct {
    texture_t* textures;
    unsigned int current_frame, frame_count;
    float timer, speed;
} sprite_t;

typedef struct {
    unsigned int id, advance;
    unsigned int size_x, size_y;
    int bearing_x, bearing_y;
} font_t;

typedef struct {
    unsigned int id;
    texture_t texture;
} framebuffer_t;

/*
---------------------
 -> Texture Struct <- 
---------------------
*/

texture_t texture_load(const char* path);
texture_t texture_from_bitmap(bitmap_t* bitmap);
texture_t texture_from_bmp(bmp_t* bmp);
texture_t texture_framebuffer(unsigned int fbo);
texture_t texture_cubemap(char** paths);
void texture_remap(texture_t* texture, bitmap_t* bitmap);

/*
-------------------------------
 -> Sprite Texture Animation <- 
-------------------------------
*/

sprite_t sprite_new(texture_t texture);
sprite_t sprite_from_bitmap(bitmap_t* bitmap);
sprite_t sprite_load(const char* path);
sprite_t sprite_load_index(const char* path);
void sprite_frame_add(sprite_t* sprite, texture_t texture);
void sprite_frame_update(sprite_t* sprite);
void sprite_free(sprite_t* sprite);

/*
-------------
 -> Skybox <- 
-------------
*/

char** skybox_load_file_paths(const char* pre_path);
void skybox_free_file_paths(char** file_paths);
skybox_t* skybox_load(const char* path);

/*
------------------
 -> Framebuffer <- 
------------------
*/

unsigned int framebuffer_id();
framebuffer_t framebuffer_new();
void framebuffer_bind(unsigned int id);

/*
-------------------
 -> Font Handler <- 
-------------------
*/

font_t* font_load(const char* path, unsigned int size);

#ifdef __cplusplus
}
#endif
#endif