#include "Tree.h"
#include "UI/UIcommon.h"

Entity player;
unsigned int playerLives;
static float camAlarm = 1.0f;

extern vec2 mouse;
extern vec4 cam;
extern wxDirectory wxDir;
extern vec2 spawnPoint;
extern unsigned int currentPlayerSprite;
extern unsigned int hp;

static wxGroup* group;

typedef enum {
    WX_GAME_MAIN_TITLE,

    WX_GAME_RECT_HEALTHBAR,

    WX_GAME_ICON_GUN,
    WX_GAME_ICON_JETPACK,
    WX_GAME_ICON_GRANADE,
    WX_GAME_ICON_HEART
} gameUIEnum;

void cameraTriggerAlarm()
{
    camAlarm = 1.0f;
}

static void cameraStep(float deltaTime)
{
    static float camera_target = 0.5f;
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};
    camAlarm -= deltaTime * 2.0f;
    
    vec2 target = *(vec2*)entity_get(player, COMPONENT_PHI_RECT);
    cam.x = lerpf(cam.x, target.x - center.x, 0.05f);
    cam.y = lerpf(cam.y, target.y - center.y, 0.05f);

    if (camAlarm < 0.0f) {
        camera_target = lerpf(camera_target, 1.0f, 0.005f);
        cam.z = lerpf(cam.z, camera_target, 0.005f);
    } else {
        camera_target = lerpf(camera_target, 0.5f, 0.01f);
        cam.z = lerpf(cam.z, camera_target, 0.01f);
    }
}

static void mouseStep()
{
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};
    mouse = vec2_add(vec2_add(vec2_div(vec2_sub(mouse, center), cam.z), center), vec2_new(cam.x, cam.y));
}

static void gameInput()
{
    if (keyboard_pressed(GLFW_KEY_ESCAPE)) {
        systemSetState(STATE_PAUSE);
        return;
    }
    if (keyboard_pressed(GLFW_KEY_R)) {
        playerReset(player);
    }
}

static void gameTick(float deltaTime) 
{
    cameraStep(deltaTime);
    mouseStep();
    playerGameStep(deltaTime);
    gunControllerStep(deltaTime);
    jetpackControllerStep();
    boxStep(deltaTime);
    gravityStep(deltaTime);
    velocityStep(deltaTime);
    smokeStep(deltaTime);
    bulletStep(deltaTime);
    granadeStep(deltaTime);
    blastStep(deltaTime);
    shadowStep(deltaTime);
    firebarrelStep();
}

static color_t color_lerp(color_t col1, color_t col2, float t)
{
    return color(
        lerpf(col1.r, col2.r, t), 
        lerpf(col1.g, col2.g, t), 
        lerpf(col1.b, col2.b, t), 
        lerpf(col1.a, col2.a, t)
    );
}

static void gameDraw()
{
    static color_t white = {1.0f, 1.0f, 1.0f, 1.0f};
    static color_t red = {1.0f, 0.0f, 0.0f, 1.0f};
    static color_t greenLight = {0.3f, 1.0f, 0.3f, 1.0f};

    wxRect* rect = group->widgets[WX_GAME_RECT_HEALTHBAR].widget;
    rect->rect.w = (float)hp * 0.5f;
    rect->color = color_lerp(red, greenLight, (float)hp * 0.01f);
    float f = (50.0f - rect->rect.w) * 0.5f;
    rect->rect.x -= f;
    wxGroupDraw(group);
    rect->rect.x += f;

    glBindVertexArray(quadVAO);
    glUseProgram(assetsGetShader(SHADER_TEXTURE));
    drawTextureColor(*assetsGetTexture(TEXTURE_AIM), mouse, white);
}

void gameStep(float deltaTime)
{
    gameInput();
    gameTick(deltaTime);
    gameDraw();
}

void gameDirectoryReset()
{
    group = &wxDir.groups[WX_DIR_GAME];
}

void gameInit()
{
    gameDirectoryReset();
    player = archetypePlayer(currentPlayerSprite);
    playerLives = 3;
}