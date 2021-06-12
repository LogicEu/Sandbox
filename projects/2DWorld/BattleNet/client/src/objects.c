#include "../BNGame.h"

void object_add_ammo(packet_t* obj)
{
    if (obj->type == TYPE_GUN) obj->slot += 14;
    else if (obj->type == TYPE_SHOTGUN) obj->slot += 8;
}

void object_shoot(list_t* bullets, array_t* obj_sprites, packet_t* used_obj, list_t* smoke)
{
    if (used_obj->type == TYPE_GUN) {
        vec2 theta = {cosf(used_obj->rot), sinf(used_obj->rot)};
        texture_t* t = texture_obj(obj_sprites, SPRITE_BULLET);
        entity_t bullet = entity_new(t->id, used_obj->x + theta.x * gun_off_aim, used_obj->y + theta.y * gun_off_aim, t->width, t->height);
        bullet.velocity.x = bullet_speed * theta.x;
        bullet.velocity.y = bullet_speed * theta.y;
        bullet.id = entity_find_new_id(bullets);
        list_push(bullets, &bullet);
        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_OTHER_SMOKE), bullet.rect.x, bullet.rect.y);
    } else if (used_obj->type == TYPE_SHOTGUN) {
        vec2 theta = {cosf(used_obj->rot), sinf(used_obj->rot)};
        theta = vec2_mult(theta, shotgun_off_aim);
        float off = -0.075f;
        for (int i = 0; i < 4; i++) {
            texture_t* t = texture_obj(obj_sprites, SPRITE_BULLET);
            entity_t bullet = entity_new(t->id, used_obj->x + theta.x, used_obj->y + theta.y, t->width, t->height);
            bullet.velocity.x = bullet_speed * cosf(used_obj->rot + off);
            bullet.velocity.y = bullet_speed * sinf(used_obj->rot + off);
            bullet.id = entity_find_new_id(bullets);
            list_push(bullets, &bullet);
            off += 0.05f;
        }
        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_OTHER_SMOKE), used_obj->x + theta.x, used_obj->y + theta.y);
    } else if (used_obj->type == TYPE_GRANADE) {
        used_obj->state = STATE_EVENT2_SEND;
    }
}

void drop_object(packet_t** used_obj, packet_t* user)
{
    (*used_obj)->state = STATE_LIVE;
    (*used_obj)->rot = 0.0f;
    (*used_obj)->count = 2;
    *used_obj = NULL;
    user->slot = NONE;
}

void pick_object(packet_t** used_obj, packet_t* obj, list_t* my_objects, packet_t* user)
{
    obj->state = STATE_CONNECT;
    if (obj->index != user->id) {
        obj->index = user->id;
        if (packet_find_by_id_ptr(my_objects, obj->id) == NULL) list_push(my_objects, &obj);
    }
    user->slot = obj->id;
    *used_obj = obj;
}

void remove_from_lists(list_t* objects, list_t* my_objects, list_t* static_objects, list_t* pre_objects, list_t* post_objects, list_t* throw_objects, uint8_t id)
{
    packet_remove_by_id_ptr(static_objects, id);
    packet_remove_by_id_ptr(post_objects, id);
    packet_remove_by_id_ptr(pre_objects, id);
    packet_remove_by_id_ptr(my_objects, id);
    packet_remove_by_id_ptr(throw_objects, id);
    packet_remove_by_id(objects, id);
}

void update_throw_object(list_t* throw_entity, list_t* throw_packets, uint8_t id, array_t* blocks, list_t* events, float delta_time)
{
    if (throw_packets->size == 0) return;
    node_t* enode = throw_entity->head;
    node_t* pnode = throw_packets->head;
    while (enode != NULL) {
        entity_t* entity = (entity_t*)enode->data;
        packet_t* obj = *(packet_t**)pnode->data;

        entity->velocity.y -= gravity;
        if (place_meeting_entity_array_offset(blocks->data, blocks->used, entity, vec2_new(entity->velocity.x * delta_time, 0.0f))) {
            entity->velocity.x = -entity->velocity.x * 0.5f;
        }
        if (place_meeting_entity_array_offset(blocks->data, blocks->used, entity, vec2_new(0.0f, entity->velocity.y * delta_time))) {
            entity->velocity.y = -entity->velocity.y * 0.5f;
        } 
        if (obj->type == TYPE_GRANADE) obj->hp --;
        obj->x += entity->velocity.x * delta_time;
        obj->y += entity->velocity.y * delta_time;
        if (obj->hp == 0) {
            if (obj->type == TYPE_GRANADE) {
                packet_t ev = packet_event(id, EVENT_EXPLOTION, obj->id, granade_hit, obj->x, obj->y, big_explotion_force);
                list_push(events, &ev);
            } else if (obj->type == TYPE_BOX) {
                obj->state = STATE_LIVE;
                obj->hp = box_hp;
            }
        }
        enode = enode->next;
        pnode = pnode->next;
    }
}

void update_used_object(packet_t** used_obj, packet_t* user, list_t* throw_objects, list_t* throw_packets, entity_t* player, array_t* obj_sprites, vec2 mouse)
{
    if (*used_obj != NULL) {
        float rads = vec2_to_rad(vec2_sub(mouse, vec2_new(player->rect.x, player->rect.y)));
        if (mouse.x < player->rect.x) (*used_obj)->count = 0;
        else (*used_obj)->count = 2;

        (*used_obj)->x = player->rect.x + cosf(rads) * used_obj_offset;
        (*used_obj)->y = player->rect.y + sinf(rads) * used_obj_offset;
        (*used_obj)->rot = rads;

        if ((*used_obj)->slot > 0 && mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            if ((*used_obj)->slot != NONE) { //Gun shoot
                user->state = STATE_EVENT1_SEND;
                (*used_obj)->slot --;
            } else { // Granade shoot
                packet_t* obj = *used_obj;
                drop_object(used_obj, user);
                obj->state = STATE_EVENT2_SEND;
                float force = granade_force;
                if (obj->type == TYPE_BOX) {
                    obj->hp = 100;
                    force = box_force;
                }
                texture_t* t = texture_obj(obj_sprites, type_to_sprite(obj->type));
                entity_t e = entity_new(0, obj->x, obj->y, t->width, t->height);
                e.rot = rads;
                e.scale = force;
                e.velocity = vec2_mult(rad_to_vec2(rads), force);
                e.id = (unsigned int)obj->id;
                list_push(throw_objects, &e);
                list_push(throw_packets, &obj);
            }
        }
    }
}