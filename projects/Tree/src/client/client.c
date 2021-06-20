#include "../TreeNet.h"

#define FILE_NET_MODULE "assets/data/.net_module.bin"
#define FILE_NET_MAP "assets/data/.net_map"

static char clientUsername[256] = "\0";
static char serverIp[16] = "\0";
static NNetHost* client = NULL;
static array_t* netEntities = NULL; 
static unsigned int received;

extern Entity player;
extern Entity usedWeapon;
extern Entity jetpack;
extern vec2 spawnPoint;

Entity netUsedWeapon;
Entity netJetpack;

extern bool stateWallSliding;
extern bool stateDashing;
extern bool stateJetpacking;

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

static void netPlayerUpdate(Entity entity, Packet* packet)
{
    rect_t* PhiRect = (rect_t*)entity_get(entity, COMPONENT_PHI_RECT);
    memcpy(&PhiRect->x, &packet->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&PhiRect->y, &packet->data[PACKET_FLOAT_Y], sizeof(float));

    rect_t* glRect = (rect_t*)entity_get(entity, COMPONENT_GL_RECT);
    unsigned int* sprite = (unsigned int*)entity_get(entity, COMPONENT_SPRITE_ID);
    unsigned int sprite_pckg = (unsigned int)packet->data[PACKET_SPRITE_ID];
    int orientation = (int)(unsigned int)packet->data[PACKET_ORIENTATION];
    orientation = orientation * 2 - 1;

    if (entity != player) {
        if (packet->data[PACKET_OP] == PACKET_OP_WALLSLIDING) {
            smokeEmit(vec2_new(PhiRect->x + (float)orientation * 8.0f, PhiRect->y - 12.0f), TEXTURE_SMOKE);
        }
        else if (packet->data[PACKET_OP] == PACKET_OP_JETPACKING) {
            smokeEmit(vec2_new(PhiRect->x, PhiRect->y - 6.0f), TEXTURE_SMOKE2);
        }
        else if (packet->data[PACKET_OP] == PACKET_OP_DASHING) {
            shadowEmit(vec2_new(PhiRect->x, PhiRect->y), (float)orientation);
        }
        else if (*sprite != SPRITE_KID_JUMPING && sprite_pckg == SPRITE_KID_JUMPING) {
            smokeEmit(vec2_new(PhiRect->x, PhiRect->y - 12.0f), TEXTURE_SMOKE);
        }
        else if (sprite_pckg == SPRITE_KID_RUNNING) {
            smokeEmit(vec2_new(PhiRect->x, PhiRect->y - 12.0f), TEXTURE_SMOKE);
        }
    }
    
    *sprite = sprite_pckg;
    if ((glRect->w > 0.0f && orientation < 0) ||
        (glRect->w < 0.0f && orientation > 0)) {
        glRect->w *= -1.0f;
    }

    memcpy(&glRect->x, &PhiRect->x, sizeof(float));
    memcpy(&glRect->y, &PhiRect->y, sizeof(float));
    sprite_frame_update(assetsGetSprite(*sprite));
}

static void netWeaponUpdate(Packet* p)
{
    uint16_t id = packetId16(p);
    Entity e = (Entity)id;
    if (e == netUsedWeapon) return;

    rect_t* r = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
    rect_t* rg = (rect_t*)entity_get(e, COMPONENT_GL_RECT);
    GunType* gun = (GunType*)entity_get(e, COMPONENT_GUN_CONTROLLER);
    float* rot = (float*)entity_get(e, COMPONENT_ROTATION);
    bool* g = (bool*)entity_get(e, COMPONENT_GRAVITY);

    unsigned int ammo = (unsigned int)p->data[PACKET_OP];
    if (ammo < gun->ammo) {
        gunShoot(e);
        gun->ammo = ammo;
    }

    if (p->data[PACKET_TYPE] == PACKET_TYPE_GUN_USED) {
        *g = false;
        gun->state = GUN_STATE_COLLECTED;
        if (fabs(*rot) > M_PI * 0.5) {
            if (rg->h > 0.0f) rg->h *= -1.0f;
        } else if (rg->h <= 0.0f) rg->h *= -1.0f;
    } else {
        *g = true;
        gun->state = GUN_STATE_LOOSE;
    }

    memcpy(&r->x, &p->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&r->y, &p->data[PACKET_FLOAT_Y], sizeof(float));
    memcpy(&rg->x, &p->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&rg->y, &p->data[PACKET_FLOAT_Y], sizeof(float));
    memcpy(rot, &p->data[PACKET_FLOAT_Z], sizeof(float));
}

static void netJetpackUpdate(Packet* p)
{
    uint16_t id = packetId16(p);
    Entity e = (Entity)id;
    if (e == netJetpack) return;

    unsigned int* state = (unsigned int*)entity_get(jetpack, COMPONENT_JETPACK);
    rect_t* r = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
    rect_t* rg = (rect_t*)entity_get(e, COMPONENT_GL_RECT);
    unsigned int* fuel = (unsigned int*)entity_get(e, COMPONENT_AMMO);
    bool* g = (bool*)entity_get(e, COMPONENT_GRAVITY);
    int orientation = (int)(unsigned int)p->data[PACKET_ORIENTATION];
    orientation = orientation * 2 - 1;

    if (p->data[PACKET_TYPE] == PACKET_TYPE_JETPACK_USED) {
        *g = false;
        *state = JETPACK_COLLECTED;
    } else {
        *g = true;
        *state = JETPACK_LOOSE;
    }

    if ((rg->w > 0.0f && orientation < 0) ||
        (rg->w < 0.0f && orientation > 0)) {
        rg->w *= -1.0f;
    }

    memcpy(&r->x, &p->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&r->y, &p->data[PACKET_FLOAT_Y], sizeof(float));
    memcpy(&rg->x, &p->data[PACKET_FLOAT_X], sizeof(float));
    memcpy(&rg->y, &p->data[PACKET_FLOAT_Y], sizeof(float));
    *fuel = (unsigned int)p->data[PACKET_OP];
}

static void packetParse(Packet* p)
{
    if (p->data[PACKET_TYPE] == PACKET_TYPE_GUN_LOOSE || p->data[PACKET_TYPE] == PACKET_TYPE_GUN_USED) {
        netWeaponUpdate(p);
    } else if (p->data[PACKET_TYPE] == PACKET_TYPE_JETPACK_LOOSE || p->data[PACKET_TYPE] == PACKET_TYPE_JETPACK_USED) {
        netJetpackUpdate(p);
    }
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
        if (p->data[PACKET_TYPE] == PACKET_TYPE_USER) {
            uint8_t id = p->data[PACKET_ID];
            Entity e = netEntityFindById(id);
            if (e) {
                if (p->data[PACKET_STATE] == NET_DISCONNECTED) disconnected = id;
                else if (p->data[PACKET_ID] != client->id) netPlayerUpdate(e, p);
            } else {
                NetEntity n = {id, archetypePlayer()};
                array_push(netEntities, &n);
                printf("Player %u has connected\n", id);
            }
        } else {
            packetParse(p);
        }
        p++;
    }

    if (!disconnected) return;
    NetEntity* n = netEntities->data;
    for (unsigned int i = 0; i < netEntities->used; i++) {
        if (n->id == disconnected) {
            entity_destroy(n->entity);
            array_remove(netEntities, i);
            break;
        }
        n++;
    }
    printf("Player %u has disconnected\n", disconnected);
}

static void netSendPlayer(Packet* packet)
{
    rect_t glRect = *(rect_t*)entity_get(player, COMPONENT_GL_RECT);
    packet->data[PACKET_ID] = client->id;
    packet->data[PACKET_STATE] = NET_CONNECTED;
    packet->data[PACKET_TYPE] = PACKET_TYPE_USER;
    memcpy(&packet->data[PACKET_FLOAT_X], &glRect.x, sizeof(float));
    memcpy(&packet->data[PACKET_FLOAT_Y], &glRect.y, sizeof(float));
    packet->data[PACKET_SPRITE_ID] = (uint8_t)*(unsigned int*)entity_get(player, COMPONENT_SPRITE_ID);
    packet->data[PACKET_ORIENTATION] = (glRect.w >= 0.0f);

    if (stateJetpacking) packet->data[PACKET_OP] = PACKET_OP_JETPACKING;
    else if (stateWallSliding) packet->data[PACKET_OP] = PACKET_OP_WALLSLIDING;
    else if (stateDashing) packet->data[PACKET_OP] = PACKET_OP_DASHING;
    else packet->data[PACKET_OP] = PACKET_OP_NONE;
}

static void netSendWeapon(Packet* packet, Entity weapon)
{
    vec2 pos = *(vec2*)entity_get(weapon, COMPONENT_PHI_RECT);
    float* rot = (float*)entity_get(weapon, COMPONENT_ROTATION);
    GunType* g = (GunType*)entity_get(weapon, COMPONENT_GUN_CONTROLLER);
    uint16_t id = (uint16_t)weapon;
    
    if (g->state == GUN_STATE_USED) packet->data[PACKET_TYPE] = PACKET_TYPE_GUN_USED;
    else packet->data[PACKET_TYPE] = PACKET_TYPE_GUN_LOOSE;
    packet->data[PACKET_OP] = (uint8_t)g->ammo;
    memcpy(&packet->data[PACKET_ENTITY_ID], &id, sizeof(uint16_t));
    memcpy(&packet->data[PACKET_FLOAT_X], &pos.x, sizeof(float));
    memcpy(&packet->data[PACKET_FLOAT_Y], &pos.y, sizeof(float));
    memcpy(&packet->data[PACKET_FLOAT_Z], rot, sizeof(float));
}

static void netSendJetpack(Packet* packet, Entity jetpack)
{
    unsigned int fuel = *(unsigned int*)entity_get(jetpack, COMPONENT_AMMO);
    vec2 pos = *(vec2*)entity_get(jetpack, COMPONENT_PHI_RECT);
    rect_t* rGl = (rect_t*)entity_get(player, COMPONENT_GL_RECT);
    uint16_t id = (uint16_t)jetpack;

    memcpy(&packet->data[PACKET_ENTITY_ID], &id, sizeof(uint16_t));
    packet->data[PACKET_TYPE] = PACKET_TYPE_JETPACK_USED;
    packet->data[PACKET_OP] = (uint8_t)fuel;
    memcpy(&packet->data[PACKET_FLOAT_X], &pos.x, sizeof(float));
    memcpy(&packet->data[PACKET_FLOAT_Y], &pos.y, sizeof(float));
    packet->data[PACKET_ORIENTATION] = (rGl->w >= 0.0f);
}

static void netSend()
{   
    if (!received) return;

    unsigned int size = 1;
    Packet* packet = (Packet*)client->buffer;
    netSendPlayer(packet);

    if (netUsedWeapon) {
        size++;
        packet++;
        netSendWeapon(packet, netUsedWeapon);
        netUsedWeapon = usedWeapon;
    }
    if (netJetpack) {
        size++;
        packet++;
        netSendJetpack(packet, netJetpack);
        netJetpack = jetpack;
    }

    NNet_send(client->server, client->packet, client->buffer, size * sizeof(Packet), 0);
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
    moduleInit();
    map_t map = map_load(FILE_NET_MAP);
    if (!map.data) {
        systemSetState(STATE_MENU);
        return;
    }
    module_from_map(&map);
    spawnPoint = map_spawn(map);
    while (spawnPoint.x < 40.0f && spawnPoint.y < 40.0f) {
        spawnPoint = map_spawn(map);
    }
    map_destroy(map);
    terrainRecalcTextures();

    netUsedWeapon = netJetpack = 0;

    netEntities = array_new(NET_MAX_CLIENT_COUNT, sizeof(NetEntity));
    client = NNetHost_create(serverIp, NET_PORT, NET_MAX_CLIENT_COUNT, NET_CHANNELS, NET_BUFFER_SIZE, NET_TIMEOUT);
    while (enet_host_service(client->host, &client->event, NET_TIMEOUT) > 0) {
        if (client->event.type == ENET_EVENT_TYPE_RECEIVE) {
            unsigned int size = NNet_read(client->event.packet, client->buffer) / sizeof(Packet);
            Packet* p = (Packet*)client->buffer;
            for (unsigned int i = 0; i < size; i++) {
                if (p->data[PACKET_TYPE] == PACKET_TYPE_USER) {
                    Entity e = archetypePlayer();
                    rect_t* r = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
                    r->x = spawnPoint.x;
                    r->y = spawnPoint.y;

                    NetEntity n = netEntityCreate(p->data[PACKET_ID], e);
                    array_push(netEntities, &n);
                    if (p->data[PACKET_STATE] == NET_CONNECTING) {
                        client->id = p->data[PACKET_ID];
                        player = n.entity;
                    }
                } else {
                    packetParse(p);
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