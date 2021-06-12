#ifndef LD48_GAME_H
#define LD48_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <fmod.h>
#include <fmod_errors.h>


#ifndef __APPLE__
#include <GL/glew.h>
#else
#define GLFW_INCLUDE_GLCOREARB
#endif
#include <GLFW/glfw3.h>

typedef struct {
    uint8_t r, g, b, a;
} pixel_t;

typedef struct {
    pixel_t* pixels;
    unsigned int width, height;
} bitmap_t;

typedef struct {
    unsigned int id, width, height;
} texture_t; 

typedef struct {
    unsigned int id, advance;
    unsigned int size_x, size_y;
    int bearing_x, bearing_y;
} font_t;

typedef struct {
    unsigned int id;
    float x, y, w, h;
} entity_t;

typedef struct {
    FMOD_SOUND* data;
    FMOD_CHANNEL* channel;
} sound_t;

#define SIZE_OF_SOUNDS 3
#define SIZE_OF_BATS 3
#define SIZE_OF_TEXTURES 22
#define SIZE_OF_GROUND 192 + SIZE_OF_BATS

typedef enum {
    STATE_PLAY,
    STATE_GAME_OVER
} state_enum;

typedef enum {
    TEXTURE_PLAYER,
    TEXTURE_GRASS_0,
    TEXTURE_GRASS_1,
    TEXTURE_GRASS_2,
    TEXTURE_GRASS_3,
    TEXTURE_BLOCK_RIGHT,
    TEXTURE_BLOCK_RIGHT_DOWN,
    TEXTURE_BLOCK_RIGHT_UP,
    TEXTURE_CORNER_RIGHT,
    TEXTURE_BLOCK_LEFT,
    TEXTURE_BLOCK_LEFT_DOWN,
    TEXTURE_BLOCK_LEFT_UP,
    TEXTURE_CORNER_LEFT,
    TEXTURE_GROUND,
    TEXTURE_BAT0,
    TEXTURE_BAT1,
    TEXTURE_BAT2,
    TEXTURE_BAT3,
    TEXTURE_BAT4,
    TEXTURE_BAT5,
    TEXTURE_BLOCK_OUT,
    TEXTURE_CLOUD
} texture_enum;

typedef enum {
    SOUND_FUNK,
    SOUND_JUMP,
    SOUND_GAME_OVER
} sound_enum;

GLFWwindow* initOpenGL(const char* title, const unsigned int width, const unsigned int height);
unsigned int quad_buffer_create();

void GLkeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int keyboard_down(unsigned int k);
unsigned int keyboard_pressed(unsigned int k);
unsigned int keyboard_released(unsigned int k);

char* shader_read_file(const char* path);
void shader_compile(const GLchar* buffer, unsigned int shader);
void shader_link(GLuint shader, unsigned int vshader, unsigned int fshader);
unsigned int shader_load(const char *vpath, const char *fpath);
void shader_set_uniform(unsigned int shader, unsigned int float_count, const char* uniform, float* data);

unsigned char* png_file_load(const char* path, unsigned int* width, unsigned int* height);
texture_t texture_load(const char* path);
texture_t texture_from_bitmap(bitmap_t* bitmap);

pixel_t* pixel_at(bitmap_t* bitmap, unsigned int x, unsigned int y);
bitmap_t* bitmap_new(unsigned int width, unsigned int height);
bitmap_t* bitmap_color(unsigned int width, unsigned int height, pixel_t pixel);
bitmap_t* bitmap_load(const char* path);
void bitmap_free(bitmap_t* bitmap);
bitmap_t* bitmap_mirror_horizontal(bitmap_t* bitmap);
bitmap_t* bitmap_mirror_vertical(bitmap_t* bitmap);

font_t* font_load(const char* path, unsigned int size);
void font_draw_char(unsigned int shader, unsigned int id, float* trans, float* scale_rot, float* color);
void font_draw_string(unsigned int shader, const char* text, font_t* font, float* color, float x, float y, float scale, float rot);

void initFMOD();
sound_t sound_load(const char* path);
void sound_play(sound_t sound);
void sound_free(sound_t sound);
void sound_stop();

void shaders_load(unsigned int* shader_texture, unsigned int* shader_font, float* res, float* cam, float* white);
texture_t* textures_load();
sound_t* sounds_load();
void sounds_free(sound_t* sounds);

entity_t entity(unsigned int id, float x, float y, float w, float h);

unsigned int point_meeting(entity_t e, float x, float y);
unsigned int point_meeting_offset(entity_t e, float x, float y, float off_x, float off_y);
unsigned int place_meeting(entity_t e1, entity_t e2);
unsigned int place_meeting_offset(entity_t e1, entity_t e2, float off_x, float off_y);
unsigned int place_meeting_entity_array(entity_t* array, unsigned int size, entity_t e, float off_x, float off_y);

entity_t* ground_init(const unsigned int width, const unsigned int height, unsigned int* size);
entity_t* bats_init(const unsigned int width);

float highscore_load(const char* path);
void highscore_save(const char* path, const char* highscore_str);

float absf(float f);
float clampf(float val, float min, float max);
float lerpf(float a, float b, float t);

#ifdef __cplusplus
}
#endif
#endif
