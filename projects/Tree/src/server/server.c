#include "../TreeNet.h"

static Packet userNew(uint8_t id)
{
    Packet p;
    memset(&p, 0, sizeof(Packet));
    p.data[PACKET_ID] = id;
    p.data[PACKET_TYPE] = PACKET_TYPE_USER;
    p.data[PACKET_STATE] = NET_CONNECTING;
    return p;
}

static void deltas_commit(array_t* deltas, queue_t* queue, Packet* packet)
{
    uint16_t id = packetId16(packet);

    Packet* p = (Packet*)deltas->data;
    for (Packet* end = p + deltas->used; p != end; p++) {
        if (packetId16(p) == id) {
            if (memcmp(p, packet, sizeof(Packet))) {
                memcpy(p, packet, sizeof(Packet));
                queue_push(queue, packet);
            }
            return;
        }
    }
    array_push(deltas, packet);
    queue_push(queue, packet);
}

int main(void)
{
    array_t* users = array_new(NET_MAX_CLIENT_COUNT, sizeof(Packet));
    array_t* deltas = array_new(64, sizeof(Packet));
    queue_t* queue = queue_new(64, sizeof(Packet));

    initNanoNet();
    NNetHost* server = NNetHost_create(NULL, NET_PORT, NET_MAX_CLIENT_COUNT, NET_CHANNELS, NET_BUFFER_SIZE, NET_TIMEOUT);
    printf("TreeNet Server is up and running.\n");
    printf("Server listening on port %u.\n", NET_PORT);

    bool userConnected = false;
    unsigned int received = 0;
    uint8_t id = 0;
    while (1) {
        while (enet_host_service(server->host, &server->event, 0) > 0) {
            switch (server->event.type) {
                case ENET_EVENT_TYPE_CONNECT: {
                    if (server->event.peer->data == NULL) {
                        id = packetNewId(users);
                        server->event.peer->data = malloc(sizeof(uint8_t));
                        memcpy(server->event.peer->data, &id, sizeof(uint8_t));

                        Packet user = userNew(id);
                        array_push(users, &user);
                    }
                    userConnected = true;
                    received++;
                    printf("User %u connected.\n", id);
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    id = *(uint8_t*)server->event.peer->data;
                    Packet* user = packetFind(users, id);
                    unsigned int size = NNet_read(server->event.packet, server->buffer) / sizeof(Packet);
                    Packet* p = (Packet*)server->buffer;
                    memcpy(user, p++, sizeof(Packet));
                    if (user->data[PACKET_STATE] == NET_CONNECTING) {
                        user->data[PACKET_STATE] = NET_CONNECTED;
                    }
                    
                    for (unsigned int i = 1; i < size; i++) {
                        deltas_commit(deltas, queue, p++);
                    }

                    received++;
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT: {
                    id = *(uint8_t*)server->event.peer->data;
                    Packet* user = packetFind(users, id);
                    user->data[PACKET_STATE] = NET_DISCONNECTED;
                    free(server->event.peer->data);
                    server->event.peer->data = NULL;
                    received++;
                    printf("User %u has disconnected.\n", id);
                    break;
                }
                case ENET_EVENT_TYPE_NONE: { break; }
                default: { break; }
            }
        }

        if (received && received >= users->used) {
            Packet* p = server->buffer, *start = users->data;
            unsigned int index = 0;
            unsigned int users_size = users->used;

            for (Packet* u = start; u != start + users_size; u++) {
                memcpy(p, u, sizeof(Packet));
                if (u->data[PACKET_STATE] == NET_DISCONNECTED) {
                    array_remove(users, index);
                } else index++;
                p++;
            }

            unsigned int objects_size = 0;
            if (userConnected) {
                objects_size = deltas->used;
                start = (Packet*)deltas->data;
                for (Packet* pp = start; pp != start + deltas->used; pp++) {
                    memcpy(p, pp, sizeof(Packet));
                    p++;
                }
                userConnected = false;
                queue->used = 0;
            } else {
                objects_size = queue->used;
                start = (Packet*)queue->data;
                while(!queue_is_empty(queue)) {
                    memcpy(p, queue_pop(queue), sizeof(Packet));
                    p++;
                }
            }

            unsigned int size = (users_size + objects_size) * sizeof(Packet);
            NNet_broadcast(server->host, server->packet, server->buffer, size, 1);
            received = 0;
        }
    }
    exitNanoNet(server->host);
    array_destroy(users);
    return 0;
}
