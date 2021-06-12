#ifndef PHOTON_PHYSICS_H
#define PHOTON_PHYSICS_H

#ifdef __cplusplus
extern "C" {
#endif

/*

==================================================

>>>>>>>>>>>>>>  PHOTON PHYSICS    >>>>>>>>>>>>>>>>

ANSI C99 header only 2D physics framework for game
development. 

=========================================  @eulogic

*/

#include <stdbool.h>
#include <Fract/Fract.h>

typedef struct {
    float x, y, w, h;
} rect_t;

typedef struct {
    unsigned int id;
    rect_t rect;
    vec2 velocity;
    float scale, rot;
} entity_t;

/*
-----------
 -> Rect <- 
-----------
*/

rect_t rect_new(float x, float y, float w, float h);
void rect_move(rect_t* r, vec2 add);

/*
-------------
 -> Entity <- 
-------------
*/

//entity_t entity_new(unsigned int id, float x, float y, float width, float height);

/*
-----------------
 -> Collisions <- 
-----------------
*/

bool point_meeting(vec2 v, rect_t r);
bool point_meeting_offset(vec2 v, rect_t r, vec2 offset);
bool point_meeting_array(vec2 v, rect_t* array, unsigned int size);
bool point_meeting_array_offset(vec2 v, rect_t* array, unsigned int size, vec2 off);

bool place_meeting(rect_t r1, rect_t r2);
bool place_meeting_offset(rect_t r1, rect_t r2, vec2 offset);
bool place_meeting_array(rect_t* array, rect_t* rect, unsigned int size);
bool place_meeting_array_offset(rect_t* array, rect_t* rect, unsigned int size, vec2 off);
//bool place_meeting_entity_array(entity_t* array, unsigned int size, entity_t* entity);
//bool place_meeting_entity_array_offset(entity_t* array, unsigned int size, entity_t* entity, vec2 offset);

#ifdef __cplusplus
}
#endif
#endif
