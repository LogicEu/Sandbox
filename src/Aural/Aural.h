#ifndef AURAL_AUDIO_H
#define AURAL_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

/*

=====================================================

>>>>>>>>>>>>>>  AURAL AUDIO LIBRARY   >>>>>>>>>>>>>>>>

ANSI C99 header only audio framework to simplify the
usage of the FMOD Audio C API and handle simple WAV IO

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
    float frequency, amplitude, angle, offset;
} sine_t;

typedef struct {
    FMOD_SOUND* data;
    FMOD_CHANNEL* channel;
} sound_t;

extern FMOD_SYSTEM *fmod_system;
extern FMOD_CHANNELGROUP* channel_group;

void initAural();
void fmod_assert(FMOD_RESULT result);

sine_t sine_create(float freq, float amp, int sample_rate);
float sine_process(sine_t* sine);

sound_t sound_load(const char* path, bool loop);
void sound_play(sound_t* sound);
void sound_free(sound_t* sound);
void sound_stop();

void wav_file_write_sine(const char* path, sine_t sine, int duration_seconds, int sample_rate, int bit_depth);

#ifdef __cplusplus
}
#endif
#endif