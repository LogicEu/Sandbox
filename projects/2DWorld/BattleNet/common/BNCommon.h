#ifndef BATTLE_NET_COMMON_H
#define BATTLE_NET_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "globals.h"
#include <Heart.h>

#define HOST "localhost" //PRIVATE STATIC IP -> PORT FORWARDED
#define PORT 7777
#define NCLIENTS 8
#define TIMEOUT 1000
#define NONE 255
#define BUFFERSIZE (NCLIENTS * sizeof(packet_t) * 4)

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 300
#define SCREEN_SCALE 2
#define SCREEN_XSCALE SCREEN_WIDTH / SCREEN_SCALE
#define SCREEN_YSCALE SCREEN_HEIGHT / SCREEN_SCALE

#define BUG printf("File: '%s', Line: '%u', Func: '%s'\n", __FILE__, __LINE__, __func__)

typedef enum event_index {
    EVENT_NONE,
    EVENT_HIT,
    EVENT_USER_HIT,
    EVENT_EXPLOTION,
    EVENT_OPEN_BOX,
    EVENT_MEDKIT,
    EVENT_AMMO,
} event_index;

typedef enum type_enum {
    TYPE_USER,
    TYPE_EVENT,
    TYPE_GUN,
    TYPE_SHOTGUN,
    TYPE_JETPACK,
    TYPE_GRANADE,
    TYPE_BOX,
    TYPE_FIRE_BARREL,
    TYPE_MEDKIT,
    TYPE_AMMO,
} type_enum;

typedef enum state_t{
    STATE_NULL,
    STATE_LIVE,
    STATE_CONNECT,
    STATE_DISCONNECT,
    STATE_EVENT1_SEND,
    STATE_EVENT1_RECEIVE,
    STATE_EVENT2_SEND,
    STATE_EVENT2_RECEIVE,
} state_t;

typedef struct packet_t {
    uint8_t id;         // byte 0
    uint8_t state;      // byte 1
    uint8_t index;      // byte 2
    uint8_t slot;       // byte 3
    uint8_t type;       // byte 4
    uint8_t count;      // byte 5
    uint8_t hp;         // byte 6
    float x;            // byte 7
    float y;            // byte 11
    float rot;          // byte 15
} packet_t;             // size 19

packet_t packet(uint8_t id, uint8_t state, float x, float y, float rot);
packet_t* packet_new(uint8_t id, uint8_t state, float x, float y, float rot);
packet_t* packet_event_new(uint8_t index, float x, float y, float rot);
packet_t* packet_find_by_id(list_t* list, uint8_t id);
packet_t* packet_find_by_id_ptr(list_t* list, uint8_t id);
uint8_t packet_find_new_id(list_t* list);
void packet_remove_by_id(list_t* list, uint8_t id);
void packet_remove_by_id_ptr(list_t* list, uint8_t id);
void packet_print(packet_t* p);
packet_t packet_event(uint8_t id, uint8_t index, uint8_t count, uint8_t hp, float x, float y, float rot);
packet_t packet_user(uint8_t id, float x, float y);
packet_t packet_gun(uint8_t id, uint8_t index, float x, float y);
packet_t packet_shotgun(uint8_t id, uint8_t index, float x, float y);
packet_t packet_granade(uint8_t id, uint8_t index, float x, float y);
packet_t packet_jetpack(uint8_t id, uint8_t index, float x, float y);
packet_t packet_firebarrel(uint8_t id, uint8_t index, float x, float y);
packet_t packet_box(uint8_t id, uint8_t index, float x, float y);
packet_t packet_medkit(uint8_t id, uint8_t index, float x, float y);
packet_t packet_ammo(uint8_t id, uint8_t index, float x, float y);
packet_t packet_roulette(list_t* objects, unsigned int index, float x, float y);

entity_t* entity_find_by_id(list_t* list, unsigned int id);
entity_t* entity_find_by_id_ptr(list_t* list, unsigned int id);
unsigned int entity_find_new_id(list_t* list);
void entity_remove_by_id(list_t* list, unsigned int id);
void entity_remove_by_id_ptr(list_t* list, unsigned int id);

void read_net_file(const char* path, char* host, uint16_t* port);

#ifdef __cplusplus
}
#endif
#endif