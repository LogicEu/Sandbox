#include "../Aural.h"

sound_t sound_load(const char* path, bool loop)
{
    sound_t sound;
    sound.channel = NULL;
    fmod_assert(FMOD_System_CreateSound(fmod_system, path, FMOD_DEFAULT, 0, &sound.data));
    if (loop == false) fmod_assert(FMOD_Sound_SetMode(sound.data, FMOD_LOOP_OFF));
    return sound;
}

void sound_play(sound_t* sound)
{
    fmod_assert(FMOD_System_PlaySound(fmod_system, sound->data, 0, false, &sound->channel));
}

void sound_stop()
{
    fmod_assert(FMOD_ChannelGroup_Stop(channel_group));
}

void sound_free(sound_t* sound)
{
    fmod_assert(FMOD_Sound_Release(sound->data));
}