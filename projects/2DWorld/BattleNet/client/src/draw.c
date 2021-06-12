#include "../BNGame.h"

void draw_peers(list_t* peers, array_t* color_players, packet_t* user)
{
    node_t* node = peers->head;
    while (node != NULL) {
        packet_t* peer = (packet_t*)node->data;
        if (peer != user) {
            sprite_t* s = sprite_by_id(color_players, peer->id, peer->index);
            texture_t* t = (texture_t*)s->textures;
            if (peer->count != IM_DEAD) {
                draw_sprite_transformed(texshader, s, peer->x, peer->y, peer->rot * t->width, t->height, 1.0f, 0.0f);
            } else draw_sprite_transformed(texshader, s, peer->x, peer->y, peer->rot * t->width, t->height, 1.0f, deg_to_rad(90.0f));
        } 
        node = node->next;
    }
}

void draw_pre_objects(list_t* objects, array_t* obj_sprites)
{
    color_t c = color(1.0f, 1.0, 1.0, 1.0);
    node_t* node = objects->head;
    while (node != NULL) {
        packet_t* obj = *(packet_t**)node->data;
        if (obj->type == TYPE_JETPACK) {
            texture_t* t = texture_obj(obj_sprites, SPRITE_JETPACK);
            draw_texture_transformed(texshader, t, c, obj->x, obj->y, ((float)obj->count - 1.0f) * t->width, t->height, 1.0f, 0.0f);
        } else {
            texture_t* t = texture_obj(obj_sprites, type_to_sprite(obj->type));
            draw_texture_transformed(texshader, t, c, obj->x, obj->y, t->width, t->height, 1.0f, 0.0f);
        }
        node = node->next;
    }
}

void draw_post_objects(list_t* objects, array_t* obj_sprites)
{
    color_t c = color(1.0f, 1.0, 1.0, 1.0);
    packet_t* obj;
    node_t* node = objects->head;
    while (node != NULL) {
        obj = *(packet_t**)node->data;
        float rot = obj->rot;
        float orient = (float)obj->count - 1.0f;
        if (obj->state == STATE_LIVE) rot = 0.0f;
        if (obj->type == TYPE_SHOTGUN) {
            texture_t* t = texture_obj(obj_sprites, SPRITE_SHOTGUN);
            draw_texture_transformed(texshader, t, c, obj->x, obj->y, t->width, orient * t->height, 0.75f, rot);
        } else {
            texture_t* t = texture_obj(obj_sprites, type_to_sprite(obj->type));
            draw_texture_transformed(texshader, t, c, obj->x, obj->y, t->width, orient * t->height, 1.0f, rot);
        }
        node = node->next;
    }
}

void draw_blocks(array_t* blocks)
{
    entity_t* e = (entity_t*)blocks->data;
    glUseProgram(texshader);
    glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
    glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
    for (entity_t* end = e + blocks->used; e != end; e++) {
        glBindTexture(GL_TEXTURE_2D, e->id);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}