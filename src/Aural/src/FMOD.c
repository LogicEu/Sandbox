#include <Aural/Aural.h>
#include <stdio.h>
#include <stdlib.h>

/*
-------------------
 -> Aural Audio  <- 
-------------------
*/

FMOD_SYSTEM *fmod_system;
FMOD_CHANNELGROUP* channel_group;

void fmod_assert(FMOD_RESULT result)
{
    if (result != FMOD_OK) {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void initAural()
{
    fmod_system = NULL;
    channel_group = NULL;
    fmod_assert(FMOD_System_Create(&fmod_system)); 
    fmod_assert(FMOD_System_Init(fmod_system, 512, FMOD_INIT_NORMAL, 0));
    fmod_assert(FMOD_System_CreateChannelGroup(fmod_system, "Group1", &channel_group));
}