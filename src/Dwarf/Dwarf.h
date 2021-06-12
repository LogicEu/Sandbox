#ifndef DWARF_DRAWINGS_H
#define DWARF_DRAWINGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Omega.h>
#include <Core/Core.h>
#include <Fract/Fract.h>
#include <Utopia/Utopia.h>
#include <Imagine/Imagine.h>
#include <Taxi/Taxi.h>
#include <Mass/Mass.h>
#include <Photon/Photon.h>

/*

======================================================

>>>>>>>>>>>>>>  DWARF DRAWING MODULE   >>>>>>>>>>>>>>>>

ANSI C99 header only drawing helper framework for Heart. 

============================================  @eulogic

*/

void light_set(unsigned int shader, vec4 light);
void draw_pixel_raw(unsigned int shader, pixel_t p, float x, float y);
void draw_pixel_raw_transformed(unsigned int shader, pixel_t p, float x, float y, float origin_x, float origin_y, float scale, float rot);
void draw_pixel(unsigned int shader, pixel_t p, float x, float y);
void draw_proto_pixel(unsigned int shader, pixel_t p, float x, float y, float origin_x, float origin_y, float scale, float rot);
void draw_texture(unsigned int shader, texture_t* texture, float x, float y, float scale, float rot);
void draw_texture_color(unsigned int shader, texture_t* texture, color_t color, float x, float y, float scale, float rot);
void draw_texture_transformed(unsigned int shader, texture_t* texture, color_t color, float x, float y, float width, float height, float scale, float rot);
void draw_font_char(unsigned int shader, unsigned int font_id, color_t color, float origin_x, float origin_y, float x, float y, float width, float height, float scale, float rot);
void draw_framebuffer(unsigned int shader, texture_t* texture);
void draw_cube(unsigned int shader, mat4 mvp, color_t color);
void draw_cube_texture(unsigned int shader, mat4 mvp, texture_t* texture);
void draw_model(unsigned int shader, model_t* model, texture_t* texture, mat4 projection, mat4 view, mat4 m, vec3 pos);
void draw_skybox(unsigned int shader, skybox_t* skybox, mat4 mvp);
void draw_mesh(unsigned int shader, unsigned int id, unsigned int size, texture_t* texture, mat4 projection, mat4 view, mat4 m, vec3 pos, unsigned int mode);
void draw_vmesh(unsigned int shader, unsigned int id, unsigned int size, texture_t* t, mat4 projection, mat4 view, mat4 m, vec3 pos, unsigned int mode);
void draw_string(unsigned int shader, const char* text, font_t* chs, float x, float y, float scale, float rot, color_t color);
void draw_font_string(unsigned int shader, const char* text, font_t* chs, float x, float y, float scale, float rot, color_t color);
void draw_bitmap(unsigned int shader, bitmap_t* bitmap, float bitmap_x, float bitmap_y, float scale, float rot);
void draw_sprite_frame(unsigned int shader, sprite_t* sprite, unsigned int frame, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot);
void draw_sprite_static(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot);
void draw_sprite(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float scale, float rot);
void draw_sprite_transformed(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot);
void draw_gizmos(unsigned int shader, rect_t r, float scale, float rot);

void vertex_element_buffer(unsigned int id, array_t* vertices, array_t* indices);
void vertex_array_buffer(unsigned int id, array_t* array, unsigned int attribute);
void vertex_element_buffer_obj(unsigned int id, array_t* vertices, array_t* indices, obj_flag type);
void mesh_bind(unsigned int id, mesh_t* mesh);
void vmesh_bind(unsigned int id, vmesh_t* mesh);
void model_bind(model_t* model, array_t* vertices, array_t* uvs, array_t* normals);

#ifdef __cplusplus
}
#endif
#endif
