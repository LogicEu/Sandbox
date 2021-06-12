#include "../BNGame.h"

array_t* blocks_load_char_map(const char* path, texture_t block, texture_t dirt_block)
{
    unsigned int width, height;
    char** map = char_map_read_file(path, &width, &height);
    array_t* blocks = array_new(32, sizeof(entity_t));
    texture_t t;
    for (int y = 0; y < height; y ++) {
        for (int x = 0; x < width; x++) {
            if (*char_map_get_char(map, x, y) == '#') {
                if ((y + 1 < height && *char_map_get_char(map, x, y + 1) != '#') || y + 1 == height) t = block;
                else t = dirt_block;
                entity_t e = entity_new(t.id, x * 32 + 16, y * 32 + 16, t.width, t.height);
                array_push(blocks, &e);
            }
        }
    }
    array_cut(blocks);
    char_map_free(map, height);
    return blocks;
}

array_t* clouds_init(sprite_t* sprite, unsigned int count)
{
    array_t* clouds = array_new(count, sizeof(entity_t));
    for (int i = 0; i < count; i++) {
        entity_t cloud = entity_new(sprite->textures->id, rand() % 1200, rand() % 900, sprite->textures->width, sprite->textures->height);
        cloud.velocity.x = ((float)(rand() % 100) + 10.) / 10.;
        cloud.scale = ((float)(rand() % 120) + 40.) / 40.;
        array_push(clouds, &cloud);
    }
    return clouds;
}