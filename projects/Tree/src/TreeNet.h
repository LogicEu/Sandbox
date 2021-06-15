#ifndef TREE_NET_H
#define TREE_NET_H

#include "Tree.h"

#define NET_MAX_CLIENT_COUNT 16
#define NET_PORT 7777
#define NET_CHANNELS 2
#define NET_BUFFER_SIZE 1024
#define NET_TIMEOUT 5000

#define PACKET_SIZE 16

typedef enum {
    NET_DISCONNECTED,
    NET_CONNECTING,
    NET_CONNECTED
} netStateEnum;

typedef struct {
    uint8_t data[PACKET_SIZE];
} Packet;

typedef enum {
    PACKET_ID = 0,
    PACKET_STATE = 1,
    PACKET_TYPE = 2,
    PACKET_OP = 3,
    PACKET_FLOAT_X = 4,
    PACKET_FLOAT_Y = 8,
    PACKET_FLOAT_Z = 12
} packetBitsEnum;

typedef enum {
    PACKET_TYPE_NONE,
    PACKET_TYPE_USER,
    PACKET_TYPE_EVENT,
    PACKET_TYPE_OBJECT,
    PACKET_TYPE_MESSAGE
} PacketEnum;

uint8_t packetNewId(array_t* users);
Packet* packetFind(array_t* packets, uint8_t id);

#endif