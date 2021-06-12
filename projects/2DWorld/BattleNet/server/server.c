#include "BNServer.h"

void object_reassign(list_t* users, packet_t* user, list_t* objects, list_t* queue_objects)
{
    node_t* node = objects->head;
    while (node != NULL) {
        packet_t* obj = (packet_t*)node->data;
        if (obj->index == user->id) {
            obj->state = STATE_LIVE;
            obj->count = 2;
            if (users->size > 1) {
                node_t* n = users->head;
                while (n != NULL) {
                    packet_t* u = (packet_t*)n->data;
                    if (user != u) obj->index = u->id;
                    n = n->next;
                }
            } else obj->index = NONE;
            list_push(queue_objects, &obj);
        }
        node = node->next;
    }
}

void object_reassign_all(list_t* users, list_t* objects)
{
    if (users->size == 0) return;
    packet_t* user = (packet_t*)users->head->data;
    node_t* node = objects->head;
    while (node != NULL) {
        packet_t* obj = (packet_t*)node->data;
        if (obj->state == STATE_LIVE) obj->index = user->id;
        node = node->next;
    }
}

void open_box(packet_t* event, list_t* objects, list_t* queue_objects, list_t* users)
{
    uint8_t index;
    if (users->head != NULL) index = ((packet_t*)users->head->data)->id;
    else index = NONE;

    unsigned int count = rand() % 3 + 1;
    float half_count = event->x - (float)count * box_weapon_offset * 0.5f;
    for (int i = 0; i < count; i++) {
        float x = half_count + (float)i * box_weapon_offset;
        packet_t p = packet_roulette(objects, index, x, event->y);
        list_push(objects, &p);
        list_push(queue_objects, &objects->tail->data);
    }
}

int main() 
{
    //zbug_init("dump/BattleNetServer.bug");
    zbug_init(NULL, 0);

    char host[16];
    uint16_t port;
    read_net_file("assets/host.net", &host[0], &port);
    
    unsigned int seed = rand_uint(rand_uint(time(NULL)));
    srand(seed);

    list_t* users = list_new(sizeof(packet_t));
    list_t* objects = list_new(sizeof(packet_t));
    list_t* events = list_new(sizeof(packet_t));
    list_t* queue_objects = list_new(sizeof(packet_t*));

    packet_t start = packet_gun(0, NONE, (float)SCREEN_XSCALE / 2.0f + 64.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_shotgun(1, NONE, (float)SCREEN_XSCALE / 2.0f + 128.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_granade(2, NONE, (float)SCREEN_XSCALE / 2.0f + 248.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_jetpack(3, NONE, (float)SCREEN_XSCALE / 2.0f - 32.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_firebarrel(4, NONE, (float)SCREEN_XSCALE / 2.0f - 32.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_box(5, NONE, (float)SCREEN_XSCALE / 2.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_medkit(6, NONE, (float)SCREEN_XSCALE / 2.0f + 324.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);
    start = packet_ammo(7, NONE, (float)SCREEN_XSCALE / 2.0f + 364.0f, (float)SCREEN_YSCALE / 2.0f);
    list_push(objects, &start);

    initNanoNet();
    NNetHost* server = NNetHost_create(NULL, port, NCLIENTS, 2, BUFFERSIZE, TIMEOUT);
    packet_t* p = NULL, *user = NULL, *obj = NULL, *ev = NULL;

    printf("Seed: %u\n", seed);
    printf("PORT: %u, HOST: %s\n", PORT, HOST);
    printf("Server listening...\n");

    uint8_t id, obj_id, user_disconnected = NONE;
    unsigned int received = 0;
    bool user_connected = false;

    while (true) {
        while (enet_host_service(server->host, &server->event, 0) > 0) {
            switch (server->event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    if (server->event.peer->data == NULL) {
                        id = packet_find_new_id(users);
                        obj_id = packet_find_new_id(objects);
                        server->event.peer->data = malloc(sizeof(uint8_t));
                        memcpy(server->event.peer->data, &id, sizeof(uint8_t));
                        
                        packet_t u = packet_user(id, (float)SCREEN_XSCALE / 2.0f, (float)SCREEN_YSCALE / 2.0f);
                        list_push(users, &u);

                        packet_t o;
                        if (id % 2 != 0) o = packet_shotgun(obj_id, u.id, u.x + 32.0f, u.y);
                        //else o = packet_granade(obj_id, u.id, u.x + 32.0f, u.y);
                        else o = packet_jetpack((uint8_t)obj_id, u.id, u.x + 32.0f, u.y);
                        list_push(objects, &o);
                        
                        received++;
                    }
                    printf("Player %u has connected\n", id);
                    if (users->size == 1) object_reassign_all(users, objects);
                    user_connected = true;
                    //printf("There are %u objects\n", objects->size);
                    break;
                
                case ENET_EVENT_TYPE_RECEIVE:
                    id = *(uint8_t*)server->event.peer->data;
                    user = packet_find_by_id(users, id);
                    unsigned int size = NNet_read(server->event.packet, server->buffer) / sizeof(packet_t);
                    p = (packet_t*)server->buffer;
                    memcpy(user, p, sizeof(packet_t));
                    p++;
                    for (packet_t* end = p + size - 1; p != end; p++) {
                        if (p->type == TYPE_EVENT) {
                            list_push(events, p);
                        } else {
                            obj = packet_find_by_id(objects, p->id);
                            //if (obj == NULL) continue;
                            if (memcmp(obj, p, sizeof(packet_t))) list_push(queue_objects, &obj);
                            memcpy(obj, p, sizeof(packet_t));
                        }
                    }
                    received++;

                    //printf("%u, %u, %f, %f, %f\n", user->id, user->state, user->x, user->y, user->rot);
                    if (user->state == STATE_CONNECT) user->state = STATE_LIVE;
                    if (user->state == STATE_EVENT1_SEND) user->state = STATE_EVENT1_RECEIVE;
                    break;

                case ENET_EVENT_TYPE_DISCONNECT:
                    id = *(uint8_t*)server->event.peer->data;
                    user = packet_find_by_id(users, id);
                    user->state = STATE_DISCONNECT;
                    free(server->event.peer->data);
                    server->event.peer->data = NULL;
                    printf("Player %u has disconnected.\n", id);
                    //printf("There are %u objects\n", objects->size);
                    user_disconnected = user->id;
                    received++;
                    break;
                
                case ENET_EVENT_TYPE_NONE:
                    break;
            }
        }

        //SEND LOOP
        if (received > 0 && received >= users->size) {
            p = (packet_t*)server->buffer;

            unsigned int users_size = users->size;
            node_t* node = users->head, *next;
            for (packet_t* end = p + users->size; p != end; p++) {
                user = (packet_t*)node->data;
                memcpy(p, user, sizeof(packet_t));
                if (user->state == STATE_DISCONNECT) {
                    object_reassign(users, user, objects, queue_objects);
                    packet_remove_by_id(users, user->id);
                }
                node = node->next;
            }

            unsigned int events_size = 0;
            node = events->head;
            while(node != NULL) {
                ev = (packet_t*)node->data;
                node_t* n = events->head;
                while(n != NULL) {
                    next = n->next;
                    if (node != n) {
                        packet_t* event = (packet_t*)n->data;
                        if (event->index == ev->index && 
                            event->count == ev->count) {
                            list_remove_node(events, n);
                        }
                    }
                    n = next;
                }
                next = node->next;
                if (ev->state == STATE_EVENT1_SEND) {
                    ev->state = STATE_EVENT1_RECEIVE;
                }
                if (ev->index == EVENT_EXPLOTION || ev->index == EVENT_MEDKIT || ev->index == EVENT_AMMO) {
                    printf("Event: %u, %u, %u, %u\n", ev->id, ev->index, ev->count, ev->state);
                    packet_remove_by_id_ptr(queue_objects, ev->count);
                    packet_remove_by_id(objects, ev->count);
                } else if (ev->index == EVENT_OPEN_BOX) {
                    printf("Opened Box: %u, %u, %u, %u\n", ev->id, ev->index, ev->count, ev->state);
                    open_box(ev, objects, queue_objects, users);
                    packet_remove_by_id_ptr(queue_objects, ev->count);
                    packet_remove_by_id(objects, ev->count);
                }
                memcpy(p, ev, sizeof(packet_t));
                list_remove_node(events, node);
                p++;
                events_size++;
                node = next;
            }

            unsigned int objects_size;
            if (user_connected) {
                objects_size = objects->size;
                node = objects->head;
                for (packet_t* end = p + objects->size; p != end; p++) {
                    obj = (packet_t*)node->data;
                    //printf("Obj: %u, %u, %u, %f, %f\n", obj->id, obj->state, obj->index, obj->x, obj->y);
                    memcpy(p, obj, sizeof(packet_t));
                    node = node->next;
                }
                user_connected = false;
            } else {
                objects_size = queue_objects->size;
                node = queue_objects->head;
                while(node != NULL) {
                    next = node->next;
                    obj = *(packet_t**)node->data;
                    //printf("Queue Obj: %u, %u, %u, %f, %f\n", obj->id, obj->state, obj->index, obj->x, obj->y);
                    memcpy(p, obj, sizeof(packet_t));
                    list_remove_node(queue_objects, node);
                    p++;
                    node = next;
                }
            }

            unsigned int size = (users_size + objects_size + events_size) * sizeof(packet_t);
            NNet_broadcast(server->host, server->packet, server->buffer, size, 1);
            received = 0;
        }
    }
    exitNanoNet(server->host);
    
    list_free(events);
    list_free(objects);
    list_free(queue_objects);
    list_free(users);
    free(events);
    free(objects);
    free(queue_objects);
    free(users);

    zbug_exit();
    return 0;
}
