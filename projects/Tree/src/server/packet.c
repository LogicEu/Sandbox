#include "../TreeNet.h"

uint8_t packetNewId(array_t* users)
{
    unsigned int i;
    for (i = 0; i < NET_MAX_CLIENT_COUNT; i++) {
        Packet* p = users->data;
        bool found = false;
        for (unsigned int j = 0; j < users->used; j++) {
            if (i == p->data[PACKET_ID]) {
                found = true;
                break;
            }
            p++;
        }
        if (!found) return i;
    }
    return i;
}

Packet* packetFind(array_t* packets, uint8_t id)
{
    Packet* p = packets->data;
    for (Packet* end = p + packets->used; p != end; p++) {
        if (p->data[PACKET_ID] == id) return p;
    }
    return NULL;
}