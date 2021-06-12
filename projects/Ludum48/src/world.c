#include "../game.h"
#include <stdlib.h>

entity_t* ground_init(const unsigned int width, const unsigned int height, unsigned int* size)
{
    entity_t* ground = (entity_t*)malloc(sizeof(entity_t) * SIZE_OF_GROUND);
    unsigned int k = 0;
    for (int x = 0; x < 14; x++) {
        if (x >= 4 && x <= 9) continue;
        for (int y = 0; y < 24; y++) {
            if (y == 0) {
                if (x == 3) ground[k++] = entity(TEXTURE_CORNER_RIGHT, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
                else if (x == 10) ground[k++] = entity(TEXTURE_CORNER_LEFT, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
                else {
                    unsigned int id = rand() % 4;
                    ground[k++] = entity(id + TEXTURE_GRASS_0, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
                }
            } else if (x == 3) ground[k++] = entity(TEXTURE_BLOCK_RIGHT, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
            else if (x == 10) ground[k++] = entity(TEXTURE_BLOCK_LEFT, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
            else ground[k++] = entity(TEXTURE_GROUND, x * 8.0f, height / 2 - y * 8.0f, 8.0f, 8.0f);
        }
    }

    for (int i = 0; i < SIZE_OF_BATS; i++) {
        float off = (float)(-18 + rand() % 36);
        ground[k++] = entity(TEXTURE_BLOCK_OUT, (float)width / 2.0f + off, 32.0f - (float)(i * 64.0f) - 8.0f, 8.0f, 8.0f);
    }

    *size = k;
    return ground;
}

entity_t* bats_init(const unsigned int width)
{
    entity_t* bats = (entity_t*)malloc(sizeof(entity_t) * SIZE_OF_BATS);
    for (int i = 0; i < SIZE_OF_BATS; i++) {
        bats[i] = entity(TEXTURE_BAT0, width / 2 - 18 + rand() % 36, 32.0f - (float)(i * 64.0f), 8.0f * (float)(1 - ((rand() % 2) * 2)), 6.0f);
    }
    return bats;
}