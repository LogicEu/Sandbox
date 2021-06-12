#include <Aural/Aural.h>
#include <stdio.h>
#include <stdlib.h>

/*
-------------------
 -> Aural Audio  <- 
-------------------
*/

static FMOD_SYSTEM *fmod_system = NULL;
static FMOD_RESULT fmod_result;
static FMOD_CHANNELGROUP* channel_group = NULL;

void initAural()
{
    fmod_result = FMOD_System_Create(&fmod_system); 
    if (fmod_result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", fmod_result, FMOD_ErrorString(fmod_result));
        exit(-1);
    }

    fmod_result = FMOD_System_Init(fmod_system, 512, FMOD_INIT_NORMAL, 0);
    if (fmod_result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", fmod_result, FMOD_ErrorString(fmod_result));
        exit(-1);
    }
    fmod_result = FMOD_System_CreateChannelGroup(fmod_system, "Group1", &channel_group);
}

sound_t* sound_load(const char* path, bool loop)
{
    sound_t* sound = (sound_t*)malloc(sizeof(sound_t));
    sound->channel = NULL;

    fmod_result = FMOD_System_CreateSound(fmod_system, path, FMOD_DEFAULT, 0, &sound->data);
    if (fmod_result != FMOD_OK) {
        printf("FMOD could not load auido file '%s'\n", path);
        return NULL;
    }
    if (loop == false) fmod_result = FMOD_Sound_SetMode(sound->data, FMOD_LOOP_OFF);
    return sound;
}

void sound_play(sound_t* sound)
{
    fmod_result = FMOD_System_PlaySound(fmod_system, sound->data, 0, false, &sound->channel);
}

void sound_stop()
{
    FMOD_ChannelGroup_Stop(channel_group);
}

void sound_free(sound_t* sound)
{
    FMOD_Sound_Release(sound->data);
    free(sound);
}