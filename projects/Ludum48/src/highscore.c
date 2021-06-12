#include "../game.h"
#include <stdio.h>

float highscore_load(const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return 0.0f;
    }

    float highscore;
    fscanf(file, "%f mts\n", &highscore);
    fclose(file);
    return highscore;
}

void highscore_save(const char* path, const char* highscore_str)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not write file '%s'\n", path);
        return;
    }
    fprintf(file, "%s\n", highscore_str);
    fclose(file);
}
