#include "Tree.h"

#define FILE_TEXTURE_WHITE "assets/sprites/WhiteBox4.png"
#define FILE_TEXTURE_TILE_1 "assets/sprites/GrassBlock2.png"
#define FILE_TEXTURE_TILE_2 "assets/sprites/DirtBlock.png"
#define FILE_TEXTURE_AIM "assets/sprites/aim.png"
#define FILE_TEXTURE_GUN "assets/sprites/Gun8.png"
#define FILE_TEXTURE_SHOTGUN "assets/sprites/Shotgun.png"
#define FILE_TEXTURE_REDBALL "assets/sprites/RedBall4.png"
#define FILE_TEXTURE_EXPLOTION "assets/sprites/explotion.png"
#define FILE_TEXTURE_CLOUD "assets/sprites/cloud.png"
#define FILE_TEXTURE_SMOKE "assets/sprites/smoke.png"
#define FILE_TEXTURE_SMOKE2 "assets/sprites/little_smoke.png"
#define FILE_TEXTURE_SMOKE3 "assets/sprites/other_smoke.png"
#define FILE_TEXTURE_JETPACK "assets/sprites/jetpack2.png"
#define FILE_TEXTURE_GRANADE "assets/sprites/grenade.png"
#define FILE_TEXTURE_BLOOD "assets/sprites/blood12.png"
#define FILE_TEXTURE_BOX "assets/sprites/box.png"
#define FILE_TEXTURE_FIREBARREL "assets/sprites/fire_barrel.png"
#define FILE_TEXTURE_MOUSE_CURSOR "assets/sprites/mouse_cursor.png"
#define FILE_TEXTURE_RIFLE "assets/sprites/machine.png"
#define FILE_TEXTURE_MACHINEGUN "assets/sprites/machinegun.png"
#define FILE_TEXTURE_FLAMETHROWER "assets/sprites/flamethrower.png"
#define FILE_TEXTURE_BAZOOKA "assets/sprites/bazooka.png"
#define FILE_TEXTURE_ROCKET "assets/sprites/rocket_small.png"
#define FILE_TEXTURE_FIRE "assets/sprites/fire16.png"

#define FILE_SPRITE_KID_RUNNING "assets/sprites/Kid/kid_index.txt"
#define FILE_TEXTURE_KID_STANDING "assets/sprites/Kid/kid_standing.png"
#define FILE_TEXTURE_KID_JUMPING "assets/sprites/Kid/kid_jumping.png"
#define FILE_TEXTURE_KID_FALLING "assets/sprites/Kid/kid_falling.png"
#define FILE_TEXTURE_KID_DEAD "assets/sprites/Kid/kid_dead_side.png"

#ifndef __APPLE__
#define FILE_PARALLAX_1 "assets/parallax/_j1.png"
#define FILE_PARALLAX_2 "assets/parallax/_j2.png"
#define FILE_PARALLAX_3 "assets/parallax/_j3.png"
#else
#define FILE_PARALLAX_1 "assets/parallax/j1.png"
#define FILE_PARALLAX_2 "assets/parallax/j2.png"
#define FILE_PARALLAX_3 "assets/parallax/j3.png"
#endif

#define FILE_FONT_1 "assets/fonts/Emulogic.ttf"

static array_t* assets;

static array_t* assetsGetArray(unsigned int index)
{
    return (array_t*)assets->data + index;
}

static void assetsLoadShaders()
{
    array_t a = array(4, sizeof(unsigned int));
    unsigned int shader = shaderLoadTexture();
    array_push(&a, &shader);
    shader = shaderLoadFont();
    array_push(&a, &shader);
    shader = shaderLoadColor();
    array_push(&a, &shader);
    shader = shaderLoadFramebuffer();
    array_push(&a, &shader);
    array_push(assets, &a);
}

static void assetsLoadTextures()
{
    array_t a = array(32, sizeof(texture_t));
    texture_t t = texture_load(FILE_TEXTURE_WHITE);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_TILE_1);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_TILE_2);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_AIM);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_GUN);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_SHOTGUN);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_REDBALL);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_EXPLOTION);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_CLOUD);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_SMOKE);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_SMOKE2);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_SMOKE3);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_JETPACK);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_GRANADE);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_BLOOD);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_BOX);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_FIREBARREL);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_MOUSE_CURSOR);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_RIFLE);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_MACHINEGUN);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_FLAMETHROWER);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_BAZOOKA);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_ROCKET);
    array_push(&a, &t);

    t = texture_load(FILE_TEXTURE_FIRE);
    array_push(&a, &t);

    array_push(assets, &a);
}

static void assetsLoadParallax()
{
    array_t a = array(3, sizeof(texture_t));
    texture_t t = texture_load(FILE_PARALLAX_1);
    array_push(&a, &t);
    t = texture_load(FILE_PARALLAX_2);
    array_push(&a, &t);
    t = texture_load(FILE_PARALLAX_3);
    array_push(&a, &t);
    array_push(assets, &a);
}

static void assetsLoadFonts()
{
    array_t a = array(1, sizeof(font_t*));
    font_t* font = font_load(FILE_FONT_1, 36);
    array_push(&a, &font);
    array_push(assets, &a);
}

static void assetsLoadFramebuffers()
{
    array_t a = array(1, sizeof(framebuffer_t));
    framebuffer_t f = framebuffer_new();
    array_push(&a, &f);
    array_push(assets, &a);
}

static void assetsLoadSprites()
{
    array_t a = array(5, sizeof(sprite_t));
    sprite_t s = sprite_load(FILE_TEXTURE_KID_STANDING);
    array_push(&a, &s);
    s = sprite_load_index(FILE_SPRITE_KID_RUNNING);
    s.speed = 0.5f;
    array_push(&a, &s);
    s = sprite_load(FILE_TEXTURE_KID_JUMPING);
    array_push(&a, &s);
    s = sprite_load(FILE_TEXTURE_KID_FALLING);
    array_push(&a, &s);
    s = sprite_load(FILE_TEXTURE_KID_DEAD);
    array_push(&a, &s);
    array_push(assets, &a);
}

static void assetsFreeSprites()
{
    array_t* a = assetsGetArray(ASSET_SPRITE);
    sprite_t* s = a->data;
    for (sprite_t* end = s + a->used; s != end; s++) {
        sprite_free(s);
    }
}

void assetsLoad()
{
    assets = array_new(4, sizeof(array_t));
    assetsLoadShaders();
    assetsLoadTextures();
    assetsLoadParallax();
    assetsLoadFonts();
    assetsLoadFramebuffers();
    assetsLoadSprites();
}

void assetsFree()
{
    assetsFreeSprites();
    free(assetsGetFont(FONT_1));
    array_t* a = (array_t*)assets->data;
    for (array_t* end = a + assets->used; a != end; a++) {
        array_free(a);
    }
    array_destroy(assets);
}

unsigned int assetsGetShader(unsigned int index)
{
    return *((unsigned int*)assetsGetArray(ASSET_SHADER)->data + index);
}

texture_t* assetsGetTexture(unsigned int index)
{
    return (texture_t*)assetsGetArray(ASSET_TEXTURE)-> data + index;
}

texture_t* assetsGetParallax(unsigned int index)
{
    return (texture_t*)assetsGetArray(ASSET_PARALLAX)-> data + index;
}

font_t* assetsGetFont(unsigned int index)
{
    return *(font_t**)assetsGetArray(ASSET_FONT)->data + index;
}

framebuffer_t* assetsGetFramebuffer(unsigned int index)
{
    return (framebuffer_t*)assetsGetArray(ASSET_FRAMEBUFFER)->data + index;
}

sprite_t* assetsGetSprite(unsigned int index)
{
    return (sprite_t*)(assetsGetArray(ASSET_SPRITE)->data) + index;
}
