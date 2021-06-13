#ifndef TREE_FRAMEWORK_H
#define TREE_FRAMEWORK_H

#ifdef __cplusplus
extern "C" {
#endif

/********************
=====================
TREE FRAMEWORK HEADER
=====================
*********************/

#include <Heart.h>

#define FULLSCREEN 0
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_SCALE 3

#define SCREEN_XSCALE SCREEN_WIDTH / SCREEN_SCALE
#define SCREEN_YSCALE SCREEN_HEIGHT / SCREEN_SCALE

#ifdef __APPLE__
#define KEY_MOD GLFW_KEY_LEFT_ALT
#else
#define KEY_MOD GLFW_KEY_LEFT_CONTROL
#endif

typedef enum {
    STATE_MENU,
    STATE_PLAY,
    STATE_PAUSE,
    STATE_LEVEL_EDITOR,
    STATE_SPRITE_EDITOR,
    STATE_UI_EDITOR,
    STATE_OPTIONS,
    STATE_NET_MENU,
    STATE_LOAD
} stateEnum;

typedef enum {
    ARCHETYPE_PLAYER,
    ARCHETYPE_TERRAIN,
    ARCHETYPE_GUN,
    ARCHETYPE_BULLET,
    ARCHETYPE_BLAST,
    ARCHETYPE_SMOKE,
    ARCHETYPE_SHADOW,
    ARCHETYPE_FIREBARREL,
    ARCHETYPE_JETPACK,
    ARCHETYPE_GRANADE
} archetypeEnum;

typedef enum assetEnum {
    ASSET_SHADER,
    ASSET_TEXTURE,
    ASSET_PARALLAX,
    ASSET_FONT,
    ASSET_FRAMEBUFFER,
    ASSET_SPRITE
} assetEnum;

typedef enum {
    SHADER_TEXTURE,
    SHADER_FONT,
    SHADER_COLOR,
    SHADER_FRAMEBUFFER_BYN,
    SHADER_WX_FONT,
    SHADER_WX_COLOR,
    SHADER_WX_TEXTURE
} shaderEnum;

typedef enum componentEnum {
    COMPONENT_PHI_RECT,
    COMPONENT_GL_RECT,
    COMPONENT_TEX_ID,
    COMPONENT_SPRITE_ID,
    COMPONENT_VEL_VEC2,
    COMPONENT_GRAVITY,
    COMPONENT_RIGID_COLLIDER,
    COMPONENT_GUN_CONTROLLER,
    COMPONENT_AMMO,
    COMPONENT_ROTATION,
    COMPONENT_SCALE,
    COMPONENT_ALPHA,
    COMPONENT_BULLET,
    COMPONENT_EXPLOTION,
    COMPONENT_SMOKE,
    COMPONENT_SHADOW,
    COMPONENT_FIREBARREL,
    COMPONENT_JETPACK,
    COMPONENT_GRANADE,
    COMPONENT_USED,
} componentEnum;

typedef enum {
    TEXTURE_WHITE,
    TEXTURE_TILE_GRASS,
    TEXTURE_TILE_DIRT,
    TEXTURE_AIM,
    TEXTURE_GUN,
    TEXTURE_SHOTGUN,
    TEXTURE_REDBALL,
    TEXTURE_EXPLOTION,
    TEXTURE_CLOUD,
    TEXTURE_SMOKE,
    TEXTURE_SMOKE2,
    TEXTURE_SMOKE3,
    TEXTURE_JETPACK,
    TEXTURE_GRANADE,
    TEXTURE_BLOOD,
    TEXTURE_BOX,
    TEXTURE_FIREBARREL,
    TEXTURE_MOUSE_CURSOR,
    TEXTURE_RIFLE,
    TEXTURE_MACHINEGUN,
    TEXTURE_FLAMETHROWER,
    TEXTURE_BAZOOKA,
    TEXTURE_ROCKET,
    TEXTURE_FIRE
} textureEnum;

typedef enum {
    SPRITE_KID_STANDING,
    SPRITE_KID_RUNNING,
    SPRITE_KID_JUMPING,
    SPRITE_KID_FALLING,
    SPRITE_KID_DEAD
} kidSpriteEnum;

typedef enum {
    PARALLAX_1,
    PARALLAX_2,
    PARALLAX_3
} parallaxEnum;

typedef enum {
    FONT_1
} fontEnum;

typedef enum {
    FRAMEBUFFER_BLACK_AND_WHITE
} framebufferEnum;

/******************/

typedef enum {
    GRANADE_COLLECTED,
    GRANADE_LOOSE,
    GRANADE_THROWED
} GranadeEnum;

typedef enum {
    GUN_STATE_LOOSE,
    GUN_STATE_COLLECTED,
    GUN_STATE_USED
} GunState;

typedef enum {
    GUN_KIND_GUN,
    GUN_KIND_SHOTGUN,
    GUN_KIND_RIFLE,
    GUN_KIND_MACHINEGUN,
    GUN_KIND_FLAMETHROWER,
    GUN_KIND_BAZOOKA
} GunKind;

typedef enum {
    BULLET_KIND_BALL,
    BULLET_KIND_ROCKET
} BulletKind;

typedef struct {
    GranadeEnum state;
    float timer;
} GranadeComponent;

typedef struct {
    GunState state;
    GunKind kind;
    unsigned int ammo; 
    float offset;
    float latencyTimer, latencySpeed;
} GunType;

/********************
=====================
TREE FRAMEWORK HEADER
=====================
*********************/

extern vec3 viewport;
extern unsigned int quadVAO;

void systemInit(unsigned int startState);
void systemStep(float deltaTime);
void systemSetState(unsigned int newState);
void systemExit();

Module moduleInit();

unsigned int shaderLoadTexture();
unsigned int shaderLoadFont();
unsigned int shaderLoadFramebuffer();
unsigned int shaderLoadColor();

void assetsLoad();
void assetsFree();
texture_t* assetsGetTexture(unsigned int index);
texture_t* assetsGetParallax(unsigned int index);
unsigned int assetsGetShader(unsigned int index);
font_t* assetsGetFont(unsigned int index);
framebuffer_t* assetsGetFramebuffer(unsigned int index);
sprite_t* assetsGetSprite(unsigned int index);

void treeInit();
void treeDirectoryReset();
void gameDirectoryReset();
void menuDirectoryReset();
void levelEditorDirectoryReset();
void spriteEditorDirectoryReset();
void UIeditorDirectoryReset();
void optionsDirectoryReset();
void netMenuDirectoryReset();

void gameStep(float deltaTime);
void gameInit();

Entity playerInit();
void playerReset();
void playerGameStep(float deltaTime);

void levelEditorStep(float deltaTime);
void levelEditorInit();

void menuStep();
void menuInit();

void netMenuInit();
void netMenuStep();

void optionsStep();
void optionsInit();

void spriteEditorInit();
void spriteEditorStep();

void UIeditorStep();
void UIeditorInit();

void loaderStep();

void UIassetsLoad();

/************
 * COMPONENTS
 * *********/

void gravityStep(float deltaTime);
void velocityStep(float deltaTime);

Entity gunCreate(vec2 position, GunKind gunKind);
void gunControllerStep(float deltaTime);
void gunPick(Entity gun);
void gunDrop(Entity gun);

void blastStep(float deltaTime);

void bulletStep(float deltaTime);

void smokeStep(float deltaTime);
void smokeEmit(vec2 position, unsigned int textureIndex);

void shadowStep(float deltaTime);
void shadowEmit(vec2 position, float side);

void firebarrelStep();

void jetpackControllerStep();
void jetpackPick(Entity jetpack);
void jetpackDrop(Entity jetpack);
void jetpackUse(Entity jetpack);

void granadeStep(float deltaTime);
void granadeCollect(Entity granade);
void granadeDrop(Entity granade);
void granadeThrow(Entity e, vec2 position);

bool checkRigidCollision(Entity entity, vec2 off);
Entity checkGunCollision(Entity entity);
Entity checkFirebarrelCollision(Entity entity);
Entity checkJetpackCollision(Entity entity);
Entity checkGranadeCollision(Entity entity);
Entity checkPhiCollision(Entity entity, Component component);
Entity checkPhiScaledCollision(Entity entity, Component component);

void drawSetCamera(float* cam);
void drawParallax(vec4 cam);
void drawComponents();
void drawTextureColor(texture_t t, vec2 pos, color_t c);
void drawTextureIdColor(unsigned int id, rect_t rect, color_t c);
void drawString(const char* text, font_t* font, float* color, float x, float y, float scale, float rot);
void drawStringCentered(const char* text, font_t* font, float* color, float x, float y, float scale, float rot);
void drawFramebuffer();
void drawRect(rect_t rect, color_t color);

void terrainReduce();
void terrainRecalcTextures();
void terraingGenRand(unsigned int width, unsigned int height);

void archetypesInit();
Entity archetypePlayer();
Entity archetypeTerrainTile(unsigned int texture_index, vec2 position);
Entity archetypeSmoke(vec2 position, unsigned int textureIndex);
Entity archetypeBlast(vec2 position, float explotionForce);
Entity archetypeFire(vec2 position, vec2 vel);
Entity archetypeBullet(vec2 position, vec2 vel, BulletKind kind);
Entity archetypeShadow(vec2 position, float side);
Entity archetypeFirebarrel(vec2 position);
Entity archetypeJetpackController(vec2 position);
Entity archetypeGranade(vec2 position);

#ifdef __cplusplus
}
#endif
#endif
