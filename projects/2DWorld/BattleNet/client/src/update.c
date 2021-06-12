#include "../BNGame.h"

void update_blood(list_t* blood, float delta_time)
{
    node_t* node = blood->head, *next;
    if (node == NULL) return;
    entity_t* e = (entity_t*)node->data;
    //texture_t* tex = (texture_t*)e->sprite->textures->first->data;
    
    glUseProgram(texshader);
    glBindTexture(GL_TEXTURE_2D, e->id);
    glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

    while (node != NULL) {
        next = node->next;
        e = (entity_t*)node->data;
        e->velocity.y -= gravity;
        e->rect.y += e->velocity.y * delta_time;
        e->rect.x += e->velocity.x * delta_time;
        e->scale -= delta_time;

        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w, e->rect.h);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);      
        
        if (e->scale < 0.0f) list_remove_node(blood, node);
        node = next;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update_static_objects(list_t* static_objects, list_t* explotions, list_t* events, list_t* my_objects, array_t* obj_sprites, uint8_t user_id)
{
    node_t* node = static_objects->head;
    if (node == NULL) return;
    while (node != NULL) {
        packet_t* p = *(packet_t**)node->data;
        if (p->hp > 0) {
            texture_t* t = texture_obj(obj_sprites, type_to_sprite(p->type));
            rect_t r = rect_new(p->x, p->y, t->width / 2.0f, t->height / 2.0f);
            node_t* n = explotions->head;
            while (n != NULL) {
                entity_t* e = (entity_t*)n->data;
                if (place_meeting(e->rect, r)) {
                    packet_t ev;
                    if (p->type == TYPE_FIRE_BARREL) {
                        ev = packet_event(user_id, EVENT_EXPLOTION, p->id, 100, p->x, p->y, big_explotion_force);
                        p->hp = 0;
                    } else {
                        int dammage = (int)e->velocity.y;
                        p->hp -= dammage * (dammage < p->hp) + p->hp * (dammage >= p->hp);
                        if (p->hp == 0) ev = packet_event(user_id, EVENT_OPEN_BOX, p->id, 100, p->x, p->y, big_explotion_force);
                    }
                    list_push(events, &ev);
                    if (packet_find_by_id_ptr(my_objects, p->id) == NULL) {
                        list_push(my_objects, node->data);
                    }
                    p->index = user_id;
                    break;
                }
                n = n->next;
            }
        }
        node = node->next;
    }
}

void update_bullets(list_t* bullets, entity_t* player, array_t* color_players, array_t* blocks, list_t* static_objects, float delta_time, list_t* events, list_t* explotions, array_t* obj_sprites, uint8_t id)
{
    node_t* node = bullets->head, *next;
    if (node == NULL) return;
    entity_t* e = (entity_t*)node->data;
    texture_t* t = texture_obj(obj_sprites, SPRITE_FIRE_BARREL);
    rect_t r = rect_new(0.0f, 0.0f, t->width / 2.0f, t->height / 2.0f);
    
    glUseProgram(texshader);
    glBindTexture(GL_TEXTURE_2D, e->id);
    glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);

    int index = 0;
    while (node != NULL) {
        next = node->next;
        
        e = (entity_t*)node->data;
        e->rect.x += e->velocity.x * delta_time * 15.0f;
        e->rect.y += e->velocity.y * delta_time * 15.0f;
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        if (place_meeting_entity_array_offset(blocks->data, blocks->used, e, vec2_new(0.0f, 0.0f))) {
            emit_explotion(explotions, sprite_obj(obj_sprites, SPRITE_EXPLOTION), e->rect.x, e->rect.y, explotion_force);
            list_remove_node(bullets, node);
        } else if (e->rot < 1.0f) {
            if (place_meeting(e->rect, player->rect)) {
                packet_t ev = packet_event(id, EVENT_USER_HIT, index, bullet_hit, e->rect.x, e->rect.y, explotion_force);
                list_push(events, &ev);
                e->rot = 6.28319f;
            } else if (static_objects->head != NULL) {
                node_t* n = static_objects->head;
                while (n != NULL) {
                    packet_t* p = *(packet_t**)n->data;
                    r.x = p->x;
                    r.y = p->y;
                    if (place_meeting(e->rect, r)) {
                        packet_t ev = packet_event(id, EVENT_HIT, index, bullet_hit, e->rect.x, e->rect.y, explotion_force);
                        list_push(events, &ev);
                        e->rot = 6.28319f;
                    }
                    n = n->next;
                }
            }
        }
        node = next;
        index++;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int update_explotions(list_t* explotions, entity_t* player, float delta_time)
{
    node_t* node = explotions->head, *next;
    if (node == NULL) return 0;
    entity_t* e = (entity_t*)node->data;
    
    glUseProgram(texshader);
    glBindTexture(GL_TEXTURE_2D, e->id);
    
    unsigned int dammage = 0;
    while (node != NULL) {
        next = node->next;   
        e = (entity_t*)node->data;
        e->velocity.y = lerpf(e->velocity.y, 0.0f, 0.1f);
        e->scale += e->velocity.y * delta_time;
        e->velocity.x -= delta_time;

        if (place_meeting(e->rect, player->rect)) {
            dammage += (int)(e->velocity.y / 2.0f);
        }

        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, e->velocity.x);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);      
        
        if (e->velocity.x < 0.0f) list_remove_node(explotions, node);
        node = next;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    return dammage;
}

void update_clouds(array_t* clouds, float delta_time)
{
    entity_t* e = (entity_t*)clouds->data;
    
    glUseProgram(texshader);
    glBindTexture(GL_TEXTURE_2D, e->id);
    glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 0.5f);
    for (entity_t* end = e + clouds->used; e != end; e++) {
        e->rect.x += e->velocity.x * delta_time;
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update_post_smoke(list_t* smoke, float delta_time)
{
    node_t* node = smoke->head, *next;
    if (node == NULL) return;
    
    glUseProgram(texshader);

    while (node != NULL) {
        next = node->next; 
        entity_t* e = (entity_t*)node->data;
        e->rect.y += delta_time * e->velocity.x * 10.0f;
        e->scale += delta_time;
        e->velocity.x -= delta_time * 0.5f;
        
        glBindTexture(GL_TEXTURE_2D, e->id);
        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, e->velocity.x);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (e->velocity.x < 0.0f) list_remove_node(smoke, node);
        node = next;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update_smoke(list_t* smoke, float delta_time)
{
    node_t* node = smoke->head, *next;
    if (node == NULL) return;
    
    glUseProgram(texshader);

    while (node != NULL) {
        next = node->next; 
        entity_t* e = (entity_t*)node->data;
        e->rect.y += delta_time * e->velocity.x * 10.0f;
        e->scale += delta_time;
        e->velocity.x -= delta_time;
        
        glBindTexture(GL_TEXTURE_2D, e->id);
        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, e->velocity.x);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        if (e->velocity.x < 0.0f) list_remove_node(smoke, node);
        node = next;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update_shadows(list_t* shadows, float delta_time)
{
    node_t* node = shadows->head, *next;
    if (node == NULL) return;
    entity_t* e = (entity_t*)node->data;
    
    glUseProgram(texshader);
    glBindTexture(GL_TEXTURE_2D, e->id);

    while (node != NULL) {
        next = node->next; 
        entity_t* e = (entity_t*)node->data;
        e->velocity.x -= delta_time;
        
        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, e->velocity.x);
        glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * e->velocity.y, e->rect.h);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);    
          
        if (e->velocity.x < 0.0f) list_remove_node(shadows, node);
        node = next;
    }
    glBindTexture(GL_TEXTURE_2D, 0);
}

void update_cam(vec2* camera, entity_t* player, float scale)
{
    glUseProgram(texshader);
    camera->x = lerpf(camera->x, player->rect.x - SCREEN_XSCALE / 2, 0.05f);
    camera->y = lerpf(camera->y, player->rect.y - SCREEN_YSCALE / 2, 0.05f);
    glUniform4f(glGetUniformLocation(texshader, "camera"), camera->x, camera->y, scale, 0.0f);
}