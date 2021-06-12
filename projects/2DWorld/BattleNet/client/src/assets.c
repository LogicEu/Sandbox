#include "../BNGame.h"

GLuint pixshader, texshader, fontshader;

void shader_init()
{
    pixshader = shader_load("assets/shaders/vert.frag", "assets/shaders/quad.frag");
    glUniform3f(glGetUniformLocation(pixshader, "resolution"), core.width, core.height, core.scale);
    glUniform4f(glGetUniformLocation(pixshader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);
    glUniform4f(glGetUniformLocation(pixshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

    texshader = shader_load("assets/shaders/texvert.frag", "assets/shaders/texfrag.frag");
    glUniform3f(glGetUniformLocation(texshader, "resolution"), core.width, core.height, core.scale);
    glUniform4f(glGetUniformLocation(texshader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);
    glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

    fontshader = shader_load("assets/shaders/fontvert.frag", "assets/shaders/fontfrag.frag");
    glUniform3f(glGetUniformLocation(fontshader, "resolution"), core.width, core.height, core.scale);
    glUniform4f(glGetUniformLocation(fontshader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);
    glUniform4f(glGetUniformLocation(fontshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
}

array_t* color_array()
{
    array_t* colors = array_new(8, sizeof(color_t));
    color_t blue = {0.2f, 0.2f, 1.0f, 1.0f};
    array_push(colors, &blue);
    color_t red = {0.7f, 0.2f, 0.2f, 1.0f};
    array_push(colors, &red);
    color_t green = {0.0f, 0.7f, 0.0f, 1.0f};
    array_push(colors, &green);
    color_t yellow = {0.8f, 0.8f, 0.0f, 1.0f};
    array_push(colors, &yellow);
    color_t violet = {0.8f, 0.0f, 0.8f, 1.0f};
    array_push(colors, &violet);
    color_t light_blue = {0.0f, 1.0f, 1.0f, 1.0f};
    array_push(colors, &light_blue);
    color_t white = {1.0f, 1.0f, 1.0f, 1.0f};
    array_push(colors, &white);
    color_t black = {0.0f, 0.0f, 0.0f, 1.0f};
    array_push(colors, &black);
    return colors;
}

array_t* load_parallax()
{
    array_t* a = array_new(3, sizeof(texture_t));
    texture_t t = texture_load("assets/parallax/j1.png");
    array_push(a, &t);
    t = texture_load("assets/parallax/j2.png");
    array_push(a, &t);
    t = texture_load("assets/parallax/j3.png");
    array_push(a, &t);
    return a;
}

bitmap_t* bitmap_reduce_alpha(const char* path)
{
    bitmap_t* b = bitmap_load(path);
    bool start = true;
    unsigned int y_min = 0, y_max = b->height;
    for (int y = 0; y < b->height; y++) {
        unsigned int check_y = false;
        for (int x = 0; x < b->width; x++) {
            if (pixel_at(b, x, y)->a != 0) {
                check_y = true;
            }
        }
        if (check_y == false) {
            if (start) y_min++;
            else y_max--;
        } else start = false;
    }

    start = true;
    unsigned int x_min = 0, x_max = b->width;
    for (int x = 0; x < b->width; x++) {
        unsigned int check_x = false;
        for (int y = 0; y < b->height; y++) {
            if (pixel_at(b, x, y)->a != 0) {
                check_x = true;
            }
        }
        if (check_x == false) {
            if (start) x_min++;
            else x_max--;
        } else start = false;
    }

    unsigned int width = x_max - x_min;
    unsigned int height = y_max - y_min;
    bitmap_t* ret = bitmap_new(width, height);
    for (int y = 0; y < ret->height; y++) {
        for (int x = 0; x < ret->width; x++) {
            memcpy(pixel_at(ret, x, y), pixel_at(b, x + x_min, y + y_min), sizeof(pixel_t));
        }
    }
    bitmap_free(b);
    return ret;
}

texture_t texture_reduce_alpha(const char* path)
{
    bitmap_t* b = bitmap_reduce_alpha(path);
    texture_t t = texture_from_bitmap(b);
    bitmap_free(b);
    return t;
}

/*sprite_t sprite_reduce_alpha(const char* path) 
{
    return sprite(texture_reduce_alpha(path));
}*/

sprite_t new_sprite_reduce_alpha(const char* path)
{
    return sprite_new(texture_reduce_alpha(path));
}

bitmap_t* bitmap_color_special(const char* path, pixel_t pcolor)
{
    bitmap_t* bitmap = bitmap_reduce_alpha(path);
    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            pixel_t* p = pixel_at(bitmap, x, y);
            if (p->b > 155 && p->r < 100) {
                memcpy(p, &pcolor, sizeof(pixel_t));
            }
        }
    }
    return bitmap;
}

texture_t load_texture_special(const char* path, pixel_t pcolor)
{
    bitmap_t* bitmap = bitmap_color_special(path, pcolor);
    texture_t texture = texture_from_bitmap(bitmap);
    return texture;
}

sprite_t load_sprite_special(const char* path, pixel_t pcolor) 
{
    return sprite_new(load_texture_special(path, pcolor));
}

array_t* load_player_sprites(color_t color)
{
    pixel_t p = ctop(color); 

    array_t* sprites = array_new(8, sizeof(sprite_t));
    sprite_t s = load_sprite_special("assets/sprites/Kid/kid_standing.png", p);
    array_push(sprites, &s);
    s = load_sprite_special("assets/sprites/Kid/kid01.png", p);
    sprite_frame_add(&s, load_texture_special("assets/sprites/Kid/kid02.png", p));
    sprite_frame_add(&s, load_texture_special("assets/sprites/Kid/kid03.png", p));
    sprite_frame_add(&s, load_texture_special("assets/sprites/Kid/kid04.png", p));
    s.speed = 0.5f;
    array_push(sprites, &s);

    s = load_sprite_special("assets/sprites/Kid/kid_jumping.png", p);
    array_push(sprites, &s);

    s = load_sprite_special("assets/sprites/Kid/kid_falling.png", p);
    array_push(sprites, &s);

    s = load_sprite_special("assets/sprites/Kid/kid_dead.png", p);
    array_push(sprites, &s);
    return sprites;
}

array_t* load_object_sprites()
{
    array_t* obj_sprites = array_new(10, sizeof(sprite_t));
    sprite_t sprite = sprite_load("assets/sprites/aim.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/GrassBlock2.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/DirtBlock.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/Gun8.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/Shotgun.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/RedBall4.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/explotion.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/cloud.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/smoke.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/little_smoke.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/other_smoke.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/jetpack2.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/grenade.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/blood12.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/box.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/fire_barrel.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/medkit.png");
    array_push(obj_sprites, &sprite);

    sprite = sprite_load("assets/sprites/bullet.png");
    array_push(obj_sprites, &sprite);

    array_cut(obj_sprites);
    return obj_sprites;
}

uint8_t type_to_sprite(uint8_t type)
{
    switch(type) {
        case TYPE_GUN:
            return SPRITE_GUN;
        case TYPE_SHOTGUN:
            return SPRITE_SHOTGUN;
        case TYPE_GRANADE:
            return SPRITE_GRANADE;
        case TYPE_JETPACK:
            return SPRITE_JETPACK;
        case TYPE_FIRE_BARREL:
            return SPRITE_FIRE_BARREL;
        case TYPE_BOX:
            return SPRITE_BOX;
        case TYPE_MEDKIT:
            return SPRITE_MEDKIT;
        case TYPE_AMMO:
            return SPRITE_AMMO;
        break;
    }
    return 0;
}

sprite_t* sprite_obj(array_t* sprites, unsigned int index)
{
    return (sprite_t*)array_index(sprites, index);
}

texture_t* texture_obj(array_t* sprites, unsigned int index)
{
    return (texture_t*)sprite_obj(sprites, index)->textures;
}

array_t* load_color_player_sprites(array_t* colors)
{
    array_t* array = array_new(colors->used, sizeof(array_t));
    color_t* c = (color_t*)colors->data;
    for (color_t* end = c + colors->used; c != end; c++) {
        array_t* color_player = load_player_sprites(*c);
        array_push(array, color_player);
        free(color_player);
    }
    return array;
}

void free_array_of_sprites(array_t* array)
{
    sprite_t* s = (sprite_t*)array->data;
    for (sprite_t* end = s + array->used; s != end; s++) {
        sprite_free(s);
    }
    array_free(array);
}

void free_array_of_array(array_t* array) 
{
    array_t* a = (array_t*)array->data;
    for (array_t* end = a + array->used; a != end; a++) {
        free_array_of_sprites(a);
    }
    array_free(array);
}

sprite_t* sprite_by_id(array_t* array, unsigned int id, unsigned int sprite_index)
{
    id %= 8;
    array_t* color_player = (array_t*)array_index(array, id);
    return (sprite_t*)array_index(color_player, sprite_index);
}