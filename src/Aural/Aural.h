#ifndef AURAL_AUDIO_H
#define AURAL_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*

=====================================================

>>>>>>>>>>>>>>  AURAL AUDIO LIBRARY   >>>>>>>>>>>>>>>>

ANSI C99 header only audio framework to simplify the
usage of the FMOD Audio C API.

============================================  @eulogic

*/

#include <Omega.h>
#include <FMOD/fmod.h>
#include <FMOD/fmod_errors.h>
#include <stdbool.h>

/*
-------------------
 -> Aural Audio  <- 
-------------------
*/

typedef struct {
    FMOD_SOUND* data;
    FMOD_CHANNEL* channel;
} sound_t;

void initAural();
sound_t* sound_load(const char* path, bool loop);
void sound_play(sound_t* sound);
void sound_free(sound_t* sound);
void sound_stop();

#ifdef __cplusplus
}
#endif
#endif