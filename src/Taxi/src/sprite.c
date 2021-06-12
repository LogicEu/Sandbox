#include <Taxi/Taxi.h>

/*
-------------------------------
 -> Sprite Texture Animation <- 
-------------------------------
*/

sprite_t sprite_new(texture_t texture)
{
    sprite_t sprite;
    sprite.frame_count = 1;
    sprite.current_frame = 0;
    sprite.speed = 0.1f;
    sprite.timer = 1.0f;
    sprite.textures = (texture_t*)malloc(sizeof(texture_t));
    memcpy(sprite.textures, &texture, sizeof(texture_t));
    return sprite;
}

sprite_t sprite_from_bitmap(bitmap_t* bitmap)
{
    return sprite_new(texture_from_bitmap(bitmap));
}

sprite_t sprite_load(const char* path)
{
    return sprite_new(texture_load(path));
}

sprite_t sprite_load_index(const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Cannot open file '%s'\n", path);
        exit(-1);
    }

    sprite_t sprite;
    sprite.frame_count = 1;
    sprite.current_frame = 0;
    sprite.speed = 0.1f;
    sprite.timer = 1.0f;
    sprite.textures = NULL;

    char string[200];
    unsigned int count = 0;
    while (1) {
        char c = fgetc(file);
        if (feof(file)) break;
        else if (c == '\n') {
            string[count] = '\0';
            const char* framepath = string;
            texture_t t = texture_load(framepath);
            if (sprite.textures == NULL) {
                sprite.textures = (texture_t*)malloc(sizeof(texture_t));
                memcpy(sprite.textures, &t, sizeof(texture_t));
            } else sprite_frame_add(&sprite, t);
            count = 0;
        } else {
            string[count] = c;
            count ++;
        }
    }
    fclose(file);
    return sprite;
}

void sprite_frame_add(sprite_t* sprite, texture_t texture)
{
    sprite->textures = (texture_t*)realloc(sprite->textures, (sprite->frame_count + 1) * sizeof(texture_t));
    memcpy(sprite->textures + sprite->frame_count, &texture, sizeof(texture_t));
    sprite->frame_count++;
}

void sprite_frame_update(sprite_t* sprite)
{
    if (sprite->frame_count <= 1) return;
    if (sprite->timer < 0.0f) {
        sprite->current_frame = ++sprite->current_frame % sprite->frame_count;
        sprite->timer = 1.0f;
    }
    sprite->timer -= sprite->speed;
}

void sprite_free(sprite_t* sprite)
{
    if (sprite->textures != NULL) free(sprite->textures);
}