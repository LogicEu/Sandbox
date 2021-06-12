#include "../BNGame.h"

void net_parse(NNetHost* client, packet_t* user, list_t* peers, list_t* objects, list_t* my_objects, list_t* post_objects, list_t* pre_objects, list_t* static_objs, list_t* events, const unsigned int size, list_t* smoke, array_t* obj_sprites, list_t* bullets)
{
    packet_t* p = (packet_t*)client->buffer, *pp;
    for (packet_t* end = p + size; p != end; p++) {
        if (p->type == TYPE_EVENT) {
            list_push(events, p);
        } else if (p->type != TYPE_USER) {
            //printf("Obj: %u, %u, %u, %f, %f\n", p->id, p->state, p->index, p->x, p->y);
            pp = packet_find_by_id(objects, p->id);
            if (pp != NULL) {
                //printf("Obj2: %u, %u, %u, %f, %f\n", pp->id, pp->state, pp->index, pp->x, pp->y);
                if (p->index != user->id) memcpy(pp, p, sizeof(packet_t));
                else if (packet_find_by_id_ptr(my_objects, p->id) == NULL) {
                    memcpy(pp, p, sizeof(packet_t));
                    list_push(my_objects, &pp);
                }
            } else {
                list_push(objects, p);
                if (p->index == user->id) list_push(my_objects, &objects->tail->data);
                if (p->type == TYPE_JETPACK || p->type == TYPE_FIRE_BARREL || p->type == TYPE_BOX) list_push(pre_objects, &objects->tail->data);
                else list_push(post_objects, &objects->tail->data);
                if (p->type == TYPE_FIRE_BARREL || p->type == TYPE_BOX) list_push(static_objs, &objects->tail->data);
            }
        } else {
            pp = packet_find_by_id(peers, p->id);
            if (pp != NULL) {
                if (pp != user) {
                    if (pp->index != KID_JUMPING && p->index == KID_JUMPING) {
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), pp->x, pp->y - 6.0f);
                    }
                    memcpy(pp, p, sizeof(packet_t));
                }
            } else if (p->state == STATE_CONNECT) {
                printf("Player %d has connected.\n", p->id);
                list_push(peers, p);
            }

            if (p->state == STATE_DISCONNECT && pp != NULL) {
                printf("Player %d disconnected.\n", p->id);
                packet_remove_by_id(peers, p->id);
            }
            if (p->state == STATE_EVENT1_RECEIVE && pp != NULL) {
                packet_t* obj = packet_find_by_id(objects, pp->slot);
                object_shoot(bullets, obj_sprites, obj, smoke);
                pp->state = STATE_LIVE;
            }
        }
    }
}

NNetHost* net_init_world(const char* host, uint16_t port, list_t* peers, list_t* events, list_t* objects, list_t* my_objects, list_t* pre_objects, list_t* post_objects, list_t* static_objects)
{
    NNetHost* client = NNetHost_create(host, port, NCLIENTS, 2, BUFFERSIZE, TIMEOUT);
    while (enet_host_service(client->host, &client->event, TIMEOUT) > 0) {
        if (client->event.type == ENET_EVENT_TYPE_RECEIVE) {
            unsigned int size = NNet_read(client->event.packet, client->buffer) / sizeof(packet_t);
            packet_t* p = (packet_t*)client->buffer;
            for (int i = 0; i < size; i++) {
                if (p->type == TYPE_EVENT) {
                    list_push(events, p);
                }
                else if (p->type != TYPE_USER) {
                    list_push(objects, p);
                    if (p->index == client->id) list_push(my_objects, &objects->tail->data);
                    if (p->type == TYPE_JETPACK || p->type == TYPE_FIRE_BARREL || p->type == TYPE_BOX) list_push(pre_objects, &objects->tail->data);
                    else list_push(post_objects, &objects->tail->data);
                    if (p->type == TYPE_FIRE_BARREL || p->type == TYPE_BOX) list_push(static_objects, &objects->tail->data);
                } else {
                    if (p->state == STATE_CONNECT) {
                        client->id = p->id;
                        p->state = STATE_LIVE;
                    }
                    list_push(peers, p);
                } 
                p++;
            }
            break;
        }
    }
    return client;
}