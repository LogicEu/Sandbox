#include "../TreeNet.h"

#define FILE_NET_MODULE "assets/data/.net_module.bin"

static char clientUsername[256] = "\0";
static char serverIp[16] = "\0";
static NNetHost* client = NULL;
static array_t* netEntities = NULL; 
static unsigned int received;

extern Entity player;

typedef struct {
    uint8_t id;
    Entity entity;
} NetEntity;

static NetEntity netEntityCreate(uint8_t id, Entity e)
{
    NetEntity n = {id, e};
    return n; 
}

static Entity netEntityFindById(uint8_t id)
{
    NetEntity* n = netEntities->data;
    for (NetEntity* end = n + netEntities->used; n != end; n++) {
        if (n->id == id) return n->entity;
    }
    return 0;
}

static void netEntityApply(Entity entity, Packet* packet)
{
    rect_t* PhiRect = (rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    rect_t* glRect = (rect_t*)entity_get(entity, COMPONENT_GL_RECT);
    memcpy(&PhiRect->x, &packet->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&PhiRect->y, &packet->data[PACKET_FLOAT_Y], sizeof(float));
    memcpy(&glRect->x, &packet->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&glRect->y, &packet->data[PACKET_FLOAT_Y], sizeof(float));
}

static void netRead()
{
    if (received) return;
    received = NNetHost_read(client, 0);
    if (!received) return;

    uint8_t disconnected = 0;
    Packet* p = client->buffer;
    unsigned int size = received / sizeof(Packet);
    for (unsigned int i = 0; i < size; i++) {
        packetPrint(p);
        uint8_t id = p->data[PACKET_ID];
        Entity e = netEntityFindById(id);
        printf("Entity %u - %u\n", e, player);
        if (e) {
            if (p->data[PACKET_STATE] == NET_DISCONNECTED) disconnected = id;
            else if (p->data[PACKET_ID] != client->id) netEntityApply(e, p);
        } else {
            NetEntity n = {id, archetypePlayer()};
            array_push(netEntities, &n);
            printf("Player %u has connected\n", id);
        }
        p++;
    }

    if (!disconnected) return;
    NetEntity* n = netEntities->data;
    for (unsigned int i = 0; i < netEntities->used; i++) {
        if (n->id == disconnected) {
            array_remove(netEntities, i);
            break;
        }
    }
    printf("Player %u has disconnected\n", disconnected);
}

static void netSend()
{   
    if (!received) return;

    Packet* packet = client->buffer;
    rect_t phiRect = *(rect_t*)entity_get(player, COMPONENT_PHI_RECT);
    packet->data[PACKET_ID] = client->id;
    packet->data[PACKET_STATE] = NET_CONNECTED;
    packet->data[PACKET_TYPE] = PACKET_TYPE_USER;
    memcpy(&packet->data[PACKET_FLOAT_X], &phiRect.x, sizeof(float));
    memcpy(&packet->data[PACKET_FLOAT_Y], &phiRect.y, sizeof(float));
    //packetPrint(packet);
    NNet_send(client->server, client->packet, client->buffer, sizeof(Packet), 0);
    received = 0;
}

void netGameStep(float deltaTime)
{
    if (!client->connected) {
        printf("You were disconnected from %s\n", serverIp);
        treeNetExit();
        return;
    }

    netRead();
    gameStep(deltaTime);
    netSend();
}

void treeNetExit()
{
    if (client->connected) NNet_disconnect(client->host, client->server, &client->event, NET_TIMEOUT);
    exitNanoNet(client->host);
    NNetHost_free(client);

    memset(serverIp, 0, 16);
    memset(clientUsername, 0, 256);
    array_destroy(netEntities);
    netEntities = NULL;
    client = NULL;
    received = 0;

    levelReset();
    systemSetState(STATE_MENU);
}

void treeNetInit(const char* username, const char* ip)
{
    strcpy(clientUsername, username);
    strcpy(serverIp, ip);
    received = 0;

    module_destroy(module_current());
    module_load(FILE_NET_MODULE);

    netEntities = array_new(NET_MAX_CLIENT_COUNT, sizeof(NetEntity));
    client = NNetHost_create(serverIp, NET_PORT, NET_MAX_CLIENT_COUNT, NET_CHANNELS, NET_BUFFER_SIZE, NET_TIMEOUT);
    while (enet_host_service(client->host, &client->event, NET_TIMEOUT) > 0) {
        if (client->event.type == ENET_EVENT_TYPE_RECEIVE) {
            unsigned int size = NNet_read(client->event.packet, client->buffer) / sizeof(Packet);
            Packet* p = (Packet*)client->buffer;
            for (unsigned int i = 0; i < size; i++) {
                if (p->data[PACKET_TYPE] == PACKET_TYPE_USER) {
                    NetEntity n = netEntityCreate(p->data[PACKET_ID], archetypePlayer());
                    array_push(netEntities, &n);
                    if (p->data[PACKET_STATE] == NET_CONNECTING) {
                        client->id = p->data[PACKET_ID];
                        player = n.entity;
                    }
                }
                p++;
            }
            received++;
            break;
        }
    }

    if (!client->connected) {
        printf("There was a problem connecting to %s\n", serverIp);
        treeNetExit();
        return;
    } 
    
    printf("You are connected to %s\n", serverIp);
    printf("Player ID: %u\n", client->id);
    systemSetState(STATE_NET_PLAY);
}