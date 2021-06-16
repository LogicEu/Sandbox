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

int main(void)
{
    array_t* users = array_new(NET_MAX_CLIENT_COUNT, sizeof(Packet));

    initNanoNet();
    NNetHost* server = NNetHost_create(NULL, NET_PORT, NET_MAX_CLIENT_COUNT, NET_CHANNELS, NET_BUFFER_SIZE, NET_TIMEOUT);
    printf("TreeNet Server is up and running.\n");
    printf("Server listening on port %u.\n", NET_PORT);

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
                    printf("User %u connected.\n", id);
                    received++;
                    break;
                }
                case ENET_EVENT_TYPE_RECEIVE: {
                    id = *(uint8_t*)server->event.peer->data;
                    Packet* user = packetFind(users, id);
                    NNet_read(server->event.packet, server->buffer);
                    memcpy(user, server->buffer, sizeof(Packet));

                    if (user->data[PACKET_STATE] == NET_CONNECTING) {
                        user->data[PACKET_STATE] = NET_CONNECTED;
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
                    printf("User %u has disconnected.\n", id);
                    received++;
                    break;
                }
                case ENET_EVENT_TYPE_NONE: { break; }
                default: { break; }
            }
        }

        if (received && received >= users->used) {
            Packet* p = server->buffer, *start = users->data;
            unsigned int index = 0;
            unsigned int size = users->used;
            for (Packet* u = start; u != start + size; u++) {
                memcpy(p, u, sizeof(Packet));
                packetPrint(p);
                if (u->data[PACKET_STATE] == NET_DISCONNECTED) {
                    array_remove(users, index);
                } else index++;
                p++;
            }

            size *= sizeof(Packet);
            NNet_broadcast(server->host, server->packet, server->buffer, size, 1);
            received = 0;
        }
    }
    exitNanoNet(server->host);
    array_destroy(users);
    return 0;
}
