#include "../game.h"
#include <stdlib.h>

texture_t* textures_load()
{
    texture_t* textures = (texture_t*)malloc(sizeof(texture_t) * SIZE_OF_TEXTURES);
    textures[TEXTURE_PLAYER] = texture_load("assets/textures/player_blue.png");
    textures[TEXTURE_GRASS_0] = texture_load("assets/textures/grass0.png");
    textures[TEXTURE_GRASS_1] = texture_load("assets/textures/grass1.png");
    textures[TEXTURE_GRASS_2] = texture_load("assets/textures/grass2.png");
    textures[TEXTURE_GRASS_3] = texture_load("assets/textures/grass3.png");
    textures[TEXTURE_BLOCK_RIGHT] = texture_load("assets/textures/block_right.png");
    textures[TEXTURE_BLOCK_RIGHT_DOWN] = texture_load("assets/textures/block_right_down.png");
    textures[TEXTURE_BLOCK_RIGHT_UP] = texture_load("assets/textures/block_right_up.png");
    textures[TEXTURE_CORNER_RIGHT] = texture_load("assets/textures/corner.png");
    textures[TEXTURE_BLOCK_LEFT] = texture_from_bitmap(bitmap_mirror_horizontal(bitmap_load("assets/textures/block_right.png")));
    textures[TEXTURE_BLOCK_LEFT_DOWN] = texture_from_bitmap(bitmap_mirror_horizontal(bitmap_load("assets/textures/block_right_down.png")));
    textures[TEXTURE_BLOCK_LEFT_UP] = texture_from_bitmap(bitmap_mirror_horizontal(bitmap_load("assets/textures/block_right_up.png")));
    textures[TEXTURE_CORNER_LEFT] = texture_from_bitmap(bitmap_mirror_horizontal(bitmap_load("assets/textures/corner.png")));
    textures[TEXTURE_GROUND] = texture_load("assets/textures/ground.png");
    textures[TEXTURE_BAT0] = texture_load("assets/textures/bat0.png");
    textures[TEXTURE_BAT1] = texture_load("assets/textures/bat1.png");
    textures[TEXTURE_BAT2] = texture_load("assets/textures/bat2.png");
    textures[TEXTURE_BAT3] = texture_load("assets/textures/bat3.png");
    textures[TEXTURE_BAT4] = texture_load("assets/textures/bat2.png");
    textures[TEXTURE_BAT5] = texture_load("assets/textures/bat1.png");
    textures[TEXTURE_BLOCK_OUT] = texture_load("assets/textures/block_out2.png");
    textures[TEXTURE_CLOUD] = texture_load("assets/textures/cloud.png");
    return textures;
}

sound_t* sounds_load()
{
    sound_t* sounds = (sound_t*)malloc(sizeof(sound_t) * SIZE_OF_SOUNDS);
    sounds[SOUND_FUNK] = sound_load("assets/sounds/funk.wav");
    sounds[SOUND_JUMP] = sound_load("assets/sounds/jump.mp3");
    sounds[SOUND_GAME_OVER] = sound_load("assets/sounds/GameOver.mp3");
    return sounds;
}

void sounds_free(sound_t* sounds)
{
    for (int i = 0; i < SIZE_OF_SOUNDS; i++) {
        sound_free(sounds[i]);
    }
    free(sounds);
}

void shaders_load(unsigned int* shader_texture, unsigned int* shader_font, float* res, float* cam, float* white)
{
    *shader_texture = shader_load("assets/shaders/texture.vert.glsl", "assets/shaders/texture.frag.glsl");
    shader_set_uniform(*shader_texture, 3, "resolution", res);
    shader_set_uniform(*shader_texture, 4, "camera", cam);
    shader_set_uniform(*shader_texture, 2, "scale_rot", &cam[2]);
    shader_set_uniform(*shader_texture, 4, "color", white);

    *shader_font = shader_load("assets/shaders/font.vert.glsl", "assets/shaders/font.frag.glsl");
    shader_set_uniform(*shader_font, 3, "resolution", res);
    shader_set_uniform(*shader_font, 4, "camera", cam);
    shader_set_uniform(*shader_font, 2, "scale_rot", &cam[2]);
    shader_set_uniform(*shader_font, 4, "color", white);
}