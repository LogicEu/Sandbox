#include "BNCommon.h"
#include <stdlib.h>

packet_t packet(uint8_t id, uint8_t state, float x, float y, float rot)
{
    packet_t packet;
    packet.id = id;
    packet.state = state;
    packet.index = 0;
    packet.slot = NONE;
    packet.type = 0;
    packet.count = 0;
    packet.hp = 0;
    packet.x = x;
    packet.y = y;
    packet.rot = rot;
    return packet;
}

packet_t* packet_new(uint8_t id, uint8_t state, float x, float y, float rot)
{
    packet_t* packet = (packet_t*)malloc(sizeof(packet_t));
    packet->id = id;
    packet->state = state;
    packet->index = 0;
    packet->slot = NONE;
    packet->type = 0;
    packet->count = 0;
    packet->hp = 0;
    packet->x = x;
    packet->y = y;
    packet->rot = rot;
    return packet;
}

packet_t* packet_event_new(uint8_t index, float x, float y, float rot)
{
    packet_t* p = packet_new(NONE, STATE_LIVE, x, y, rot);
    p->type = TYPE_EVENT;
    p->index = index;
    p->count = NONE;
    return p;
}

packet_t* packet_find_by_id(list_t* list, uint8_t id)
{
    node_t* node = list->head;
    while (node != NULL) {
        packet_t* p = (packet_t*)node->data;
        if (id == p->id) return p;
        node = node->next;
    }
    return NULL;
}

packet_t* packet_find_by_id_ptr(list_t* list, uint8_t id)
{
    node_t* node = list->head;
    while (node != NULL) {
        packet_t* p = *(packet_t**)node->data;
        if (id == p->id) return p;
        node = node->next;
    }
    return NULL;
}

uint8_t packet_find_new_id(list_t* list)
{
    uint8_t ret = 0;
    while (packet_find_by_id(list, ret) != NULL) {
        ret++;
    }
    return ret;
}

void packet_remove_by_id(list_t* list, uint8_t id) 
{
    node_t* node = list->head, *next;
    while (node != NULL) {
        packet_t* p = (packet_t*)node->data;
        next = node->next;
        if (p->id == id) {
            list_remove_node(list, node);
            break;
        }
        node = next;
    }
}

void packet_remove_by_id_ptr(list_t* list, uint8_t id) 
{
    node_t* node = list->head, *next;
    while (node != NULL) {
        packet_t* p = *(packet_t**)node->data;
        next = node->next;
        if (p->id == id) {
            list_remove_node(list, node);
            break;
        }
        node = next;
    }
}

void packet_print(packet_t* p)
{
    printf("Id: %u, State: %u, Index: %u, Slot: %u, x: %f, y: %f\n", p->id, p->state, p->index, p->slot, p->x, p->y);
}

packet_t packet_event(uint8_t id, uint8_t index, uint8_t count, uint8_t hp, float x, float y, float rot)
{
    packet_t p = packet(id, STATE_EVENT1_SEND, x, y, rot);
    p.type = TYPE_EVENT;
    p.index = index;
    p.count = count;
    p.hp = hp;
    return p;
}

packet_t packet_user(uint8_t id, float x, float y)
{
    packet_t packet;
    packet.id = id; //user id
    packet.state = STATE_CONNECT; //user just connected
    packet.index = 0; //sprite index 0
    packet.slot = NONE; //object slot 
    packet.type = TYPE_USER; 
    packet.count = 0; //secondary state messages
    packet.hp = 100; //user hp
    packet.x = x; // x position
    packet.y = y; // y position
    packet.rot = 1.0f; //orientation
    return packet;
}

packet_t packet_gun(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live
    packet.index = index; //parent user
    packet.slot = gun_ammo; //ammo
    packet.type = TYPE_GUN; //what object is it
    packet.count = 2; //orientation
    packet.hp = 100; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_shotgun(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live
    packet.index = index; //parent user
    packet.slot = shotgun_ammo; //ammo
    packet.type = TYPE_SHOTGUN; //what object is it
    packet.count = 2; //orientation
    packet.hp = 100; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_granade(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_GRANADE; //what object is it
    packet.count = 2; //orientation
    packet.hp = granade_timer; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_jetpack(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_JETPACK; //what object is it
    packet.count = 2; //orientation
    packet.hp = jetpack_fuel; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_firebarrel(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_FIRE_BARREL; //what object is it
    packet.count = 2; //orientation
    packet.hp = 100; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_box(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_BOX; //what object is it
    packet.count = 2; //orientation
    packet.hp = box_hp; //Timers
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_medkit(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_MEDKIT; //what object is it
    packet.count = 2; //orientation
    packet.hp = 25; //HP
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_ammo(uint8_t id, uint8_t index, float x, float y)
{
    packet_t packet;
    packet.id = id; //object id
    packet.state = STATE_LIVE; //always start with live -> used when fired
    packet.index = index; //parent user
    packet.slot = NONE; //object with no ammo
    packet.type = TYPE_AMMO; //what object is it
    packet.count = 2; //orientation
    packet.hp = 25; //HP
    packet.x = x; //x position
    packet.y = y; //y position
    packet.rot = 0.0f; //rads when used
    return packet;
}

packet_t packet_roulette(list_t* objects, unsigned int index, float x, float y)
{
    unsigned int dice = rand() % 6;
    packet_t p;
    switch (dice) {
        case 0:
            p = packet_gun(packet_find_new_id(objects), index, x, y);
            break;
        case 1:
            p = packet_shotgun(packet_find_new_id(objects), index, x, y);
            break;
        case 2:
            p = packet_granade(packet_find_new_id(objects), index, x, y);
            break;
        case 3:
            p = packet_jetpack(packet_find_new_id(objects), index, x, y);
            break;
        case 4:
            p = packet_medkit(packet_find_new_id(objects), index, x, y);
            break;
        case 5:
            p = packet_ammo(packet_find_new_id(objects), index, x, y);
            break;
    }
    return p;
}