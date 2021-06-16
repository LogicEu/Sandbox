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

void packetPrint(Packet* p)
{
    printf(
        "Packet -> Id: %u, Type: %u, State: %u, X: %f, Y: %f\n", 
        p->data[PACKET_ID], 
        p->data[PACKET_TYPE], 
        p->data[PACKET_STATE],
        *(float*)&p->data[PACKET_FLOAT_X], 
        *(float*)&p->data[PACKET_FLOAT_Y]
    );
}