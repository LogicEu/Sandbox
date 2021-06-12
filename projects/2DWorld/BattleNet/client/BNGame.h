#ifndef BATTLE_NET_GAME_H
#define BATTLE_NET_GAME_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include "../common/BNCommon.h"

typedef enum count_enum {
    IM_NOTHING,
    IM_DASHING,
    IM_SLIDING,
    IM_DEAD,
    IM_JETPACKING
} count_enum;

typedef enum sprite_enum {
    KID_STANDING,
    KID_RUNNING,
    KID_JUMPING,
    KID_FALLING,
    KID_DEAD
} sprite_enum;

typedef enum obj_sprite_enum {
    SPRITE_AIM,
    SPRITE_BLOCK,
    SPRITE_DIRT_BLOCK,
    SPRITE_GUN,
    SPRITE_SHOTGUN,
    SPRITE_BULLET,
    SPRITE_EXPLOTION,
    SPRITE_CLOUD,
    SPRITE_SMOKE,
    SPRITE_LITTLE_SMOKE,
    SPRITE_OTHER_SMOKE,
    SPRITE_JETPACK,
    SPRITE_GRANADE,
    SPRITE_BLOOD,
    SPRITE_BOX,
    SPRITE_FIRE_BARREL,
    SPRITE_MEDKIT,
    SPRITE_AMMO
} obj_sprite_enum;

extern GLuint pixshader, texshader, fontshader;

void shader_init();
array_t* color_array();
array_t* load_parallax();
bitmap_t* bitmap_reduce_alpha(const char* path);
texture_t texture_reduce_alpha(const char* path);
sprite_t new_sprite_reduce_alpha(const char* path);
bitmap_t* bitmap_color_special(const char* path, pixel_t pcolor);
texture_t load_texture_special(const char* path, pixel_t pcolor);
sprite_t load_sprite_special(const char* path, pixel_t pcolor);
array_t* load_player_sprites(color_t color);
array_t* load_object_sprites();
uint8_t type_to_sprite(uint8_t type);
sprite_t* sprite_obj(array_t* sprites, unsigned int index);
texture_t* texture_obj(array_t* sprites, unsigned int index);
array_t* load_color_player_sprites(array_t* colors);
void free_array_of_sprites(array_t* array);
void free_array_of_array(array_t* array);
sprite_t* sprite_by_id(array_t* array, unsigned int id, unsigned int sprite_index);

char* char_map_get_char(char** array, size_t offx, size_t offy);
char** char_map_init(size_t width, size_t height);
char** char_map_read_file(const char* path, unsigned int* widthptr, unsigned int* heightptr);
void char_map_write_file(char** map, size_t width, size_t height, const char* path);
void char_map_print(char** map, unsigned int width, unsigned int height);
void map_char_print_file(const char* path);
void char_map_free(char** map, size_t height);

array_t* buffer_load_array(const char* path, unsigned int bytes);
void buffer_write_array(const char* path, array_t* array);

array_t* blocks_load_char_map(const char* path, texture_t block, texture_t dirt_block);
array_t* clouds_init(sprite_t* sprite, unsigned int count);

void emit_smoke(list_t* smoke, sprite_t* smoke_sprite, float x, float y);
void emit_big_smoke(list_t* smoke, sprite_t* smoke_sprite, float x, float y);
void emit_blood(list_t* blood, sprite_t* blood_sprite, unsigned int count, float x, float y);
void emit_shadow(list_t* shadows, sprite_t* sprite, float x, float y, float rot);
void emit_explotion(list_t* explotions, sprite_t* exp_sprite, float x, float y, float exp_force);

void draw_parallax(array_t* images, vec2 cam, vec2 center);

void draw_peers(list_t* peers, array_t* color_players, packet_t* user);
void draw_pre_objects(list_t* objects, array_t* obj_sprites);
void draw_post_objects(list_t* objects, array_t* obj_sprites);
void draw_blocks(array_t* blocks);

void update_blood(list_t* blood, float delta_time);
void update_static_objects(list_t* static_objects, list_t* explotions, list_t* events, list_t* my_objects, array_t* obj_sprites, uint8_t user_id);
void update_bullets(list_t* bullets, entity_t* player, array_t* color_players, array_t* blocks, list_t* static_objects, float delta_time, list_t* events, list_t* explotions, array_t* obj_sprites, uint8_t id);
unsigned int update_explotions(list_t* explotions, entity_t* player, float delta_time);
void update_clouds(array_t* clouds, float delta_time);
void update_post_smoke(list_t* smoke, float delta_time);
void update_smoke(list_t* smoke, float delta_time);
void update_shadows(list_t* shadows, float delta_time);
void update_cam(vec2* camera, entity_t* player, float scale);

void net_parse(NNetHost* client, packet_t* user, list_t* peers, list_t* objects, list_t* my_objects, list_t* post_objects, list_t* pre_objects, list_t* static_objs, list_t* events, const unsigned int size, list_t* smoke, array_t* obj_sprites, list_t* bullets);
NNetHost* net_init_world(const char* host, uint16_t port, list_t* peers, list_t* events, list_t* objects, list_t* my_objects, list_t* pre_objects, list_t* post_objects, list_t* static_objects);

void object_add_ammo(packet_t* obj);
void object_shoot(list_t* bullets, array_t* obj_sprites, packet_t* used_obj, list_t* smoke);
void drop_object(packet_t** used_obj, packet_t* user);
void pick_object(packet_t** used_obj, packet_t* obj, list_t* my_objects, packet_t* user);
void remove_from_lists(list_t* objects, list_t* my_objects, list_t* static_objects, list_t* pre_objects, list_t* post_objects, list_t* throw_objects, uint8_t id);
void update_throw_object(list_t* throw_entity, list_t* throw_packets, uint8_t id, array_t* blocks, list_t* events, float delta_time);
void update_used_object(packet_t** used_obj, packet_t* user, list_t* throw_objects, list_t* throw_packets, entity_t* player, array_t* obj_sprites, vec2 mouse);

void player_respawn(entity_t* player, packet_t* user, texture_t* t);

#ifdef __cplusplus
}
#endif
#endif