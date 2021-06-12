#include <Aural/Aural.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
    initAural();
    sound_t* sound = sound_load("assets/sounds/jump.mp3", false);
    if (!sound) return -1;

    while(1) {
        sleep(1);
        sound_play(sound);
        printf("Sound playing...\n");
    }

    sound_free(sound);
    return 0;
}
