#ifndef CORE_GEAR_H
#define CORE_GEAR_H

#ifdef __cplusplus
extern "C" {
#endif

/*

=====================================================

>>>>>>>>>>>>>>  CORE GEAR FRAMEWORK   >>>>>>>>>>>>>>>>

ANSI C99 header only graphic library abstracting OpenGL
into a simpler framework.

============================================  @eulogic

*/

#include <Omega.h>

#ifndef __APPLE__
    #include <GL/glew.h>
#else
    #define GLFW_INCLUDE_GLCOREARB
#endif

#include <GLFW/glfw3.h>

/*
-----------
 -> Core <- 
-----------
*/

void coreInit();
void coreExit();
void core_set_3d_mode();
void core_set_2d_mode();
void core_set_2d_scale(unsigned int scale);
unsigned int core_get_2d_scale();
void core_get_size(unsigned int* width, unsigned int* height);
unsigned int core_need_update();
void core_update();

/*
-------------
 -> Window <- 
-------------
*/

GLFWwindow* window_get();
void window_create(const char* title, int width, int height, unsigned int fullscreen, unsigned int resizable);
void window_set_size(int width, int height);
void window_get_size(int* width, int* height);
void window_set_position(int x, int y);
void window_get_position(int* x, int* y);
unsigned int window_is_open();

unsigned int window_file_has();
char* window_file_get();

/*
-------------
 -> Screen <- 
-------------
*/

void screen_color(float r, float g, float b, float a);
void screen_clear();
void screen_refresh();

/*
--------------
 -> Shaders <- 
--------------
*/

char* shader_read_file(const char* path);
char* shader_parse(const char* path);
void shader_compile(const GLchar* buffer, unsigned int shader);
void shader_link(GLuint shader, unsigned int vshader, unsigned int fshader);
unsigned int shader_load(const char *vpath, const char *fpath);
void shader_set_uniform(unsigned int shader, unsigned int float_count, const char* uniform, float* data);

/*
------------------
 -> Buffer Bind <- 
------------------
*/

unsigned int vertex_array_buffer_id_new();
void vertex_array_buffer_attribute_set(unsigned int attribute, unsigned int float_pack, unsigned int stride, size_t offset);
void vertex_array_buffer_create(unsigned int id, void* buffer, unsigned int size);
void vertex_array_buffer_indexed_create(unsigned int id, void* buffer, unsigned int buffer_size, unsigned int* indices, unsigned int indices_size);
unsigned int vertex_array_buffer_quad_create();
unsigned int vertex_array_buffer_voxel_create();
unsigned int vertex_array_buffer_skybox_create();
void vertex_array_buffer_voxel_tex_coord(unsigned int id);

/*
-----------------
 -> Time Input <- 
-----------------
*/

float time_start_timer();
float time_end_timer(float start);
float time_get_delta_time(float* last_time);

/*
------------------
 -> Mouse Input <- 
------------------
*/

void mouse_get_position(float* x, float* y);
void mouse_get_position3d(float* x, float* y);
unsigned int mouse_down(unsigned int button);
unsigned int mouse_pressed(unsigned int button);
unsigned int mouse_released(unsigned int button);
void mouse_set_mode(unsigned int mouse_lock);

/*
---------------------
 -> Keyboard Input <- 
---------------------
*/

unsigned int keyboard_down(unsigned int k);
unsigned int keyboard_pressed(unsigned int k);
unsigned int keyboard_released(unsigned int k);
char keyboard_char();

#ifdef __cplusplus
}
#endif
#endif
