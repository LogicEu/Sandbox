#include "Tree.h"

#ifdef __APPLE__ 
    #define FB_SIZE 2
#else 
    #define FB_SIZE 1
#endif

static unsigned int state;
static unsigned int oldState;

unsigned int randSeed;
unsigned int quadVAO;

extern unsigned int hp;
extern wxDirectory wxDir;
extern vec2 spawnPoint;

vec2 mouse;
vec3 viewport;
vec4 cam;

bool blackAndWhite;

static void systemSnapshot()
{
    bmp_t tmp = bmp_new(SCREEN_WIDTH * FB_SIZE, SCREEN_HEIGHT * FB_SIZE, 4);
    bmp_t bmp = bmp_flip_vertical(&tmp);
    bmp_free(&tmp);
    glReadPixels(0, 0, bmp.width, bmp.height, GL_RGBA, GL_UNSIGNED_BYTE, &bmp.pixels[0]);
    bmp_write("test.png", &bmp);
    bmp_free(&bmp);
}

static void systemDraw()
{
    glUseProgram(assetsGetShader(SHADER_TEXTURE));
    drawParallax(cam);
    drawSetCamera(&cam.x);
    if (state == STATE_PLAY || state == STATE_LEVEL_EDITOR || state == STATE_PAUSE || state == STATE_NET_PLAY) drawComponents();
}

static void systemPause(float deltaTime)
{
    if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)   ||
        keyboard_pressed(GLFW_KEY_SPACE)        || 
        keyboard_pressed(GLFW_KEY_BACKSPACE)    || 
        keyboard_pressed(GLFW_KEY_N)) {
        blackAndWhite = false;
        state = oldState;
    }
    if (keyboard_pressed(GLFW_KEY_ESCAPE) || keyboard_pressed(GLFW_KEY_Y)) {
        if (oldState == STATE_PLAY || oldState == STATE_LEVEL_EDITOR) systemSetState(STATE_MENU);
        else if (oldState == STATE_NET_PLAY) treeNetExit();
        else systemExit();
    }
    if (keyboard_pressed(GLFW_KEY_M)) {
        systemSetState(STATE_MENU);
    }
    if (oldState == STATE_NET_PLAY) netGameStep(deltaTime);
}

static void systemInput()
{
    if (keyboard_down(GLFW_KEY_LEFT_CONTROL)) {
        if (keyboard_pressed(GLFW_KEY_M)) {
            systemSetState(STATE_MENU);
            return;
        }
        if (keyboard_pressed(GLFW_KEY_G)) {
            systemSetState(STATE_PLAY);
            return;
        }
        if (keyboard_pressed(GLFW_KEY_E)) {
            systemSetState(STATE_LEVEL_EDITOR);
            return;
        }
        if (keyboard_pressed(GLFW_KEY_U)) {
            systemSetState(STATE_UI_EDITOR);
            return;
        }
        if (keyboard_pressed(GLFW_KEY_B)) {
            if (blackAndWhite) blackAndWhite = false;
            else blackAndWhite = true;
        }
    }
}

void systemStep(float deltaTime)
{
    if (blackAndWhite) framebuffer_bind(assetsGetFramebuffer(FRAMEBUFFER_BLACK_AND_WHITE)->id);
    else framebuffer_bind(0);
    screen_clear();

    systemDraw();
    systemInput();
    mouse_get_position(&mouse.x, &mouse.y);
    switch (state) {
        case STATE_MENU:
            menuStep();
            break;
        case STATE_PLAY:
            gameStep(deltaTime);
            break;
        case STATE_PAUSE:
            systemPause(deltaTime);
            break;
        case STATE_LEVEL_EDITOR:
            levelEditorStep(deltaTime);
            break;
        case STATE_UI_EDITOR:
            UIeditorStep();
            break;
        case STATE_SPRITE_EDITOR:
            spriteEditorStep();
            break;
        case STATE_OPTIONS:
            optionsStep();
            break;
        case STATE_NET_MENU:
            netMenuStep();
            break;
        case STATE_NET_PLAY:
            netGameStep(deltaTime);
            break;
        case STATE_LOAD:
            //loaderStep();
            systemSetState(STATE_MENU);
            break;
        default:
            break;
    }
    if (keyboard_down(GLFW_KEY_LEFT_CONTROL) && keyboard_pressed(GLFW_KEY_P)) {
        systemSnapshot();
    }
    if (blackAndWhite) drawFramebuffer();
    screen_refresh();
}

void systemSetState(unsigned int newState)
{
    if (newState == state) return;
    oldState = state;
    state = newState;

    if (oldState == STATE_MENU) {
        cam = vec4_new(0.0f, 0.0f, 1.0f, 0.0f);
    }
    if (state == STATE_LEVEL_EDITOR) {
        cam.x = spawnPoint.x - (viewport.x / viewport.z) * 0.5;
        cam.y = spawnPoint.y - (viewport.y / viewport.z) * 0.5;
        cam.z = 1.0f;
    }

    if (state == STATE_PAUSE) blackAndWhite = true;
    if (oldState == STATE_PAUSE) blackAndWhite = false;
}

Module moduleInit()
{
    return module_create(   
        19, 
        sizeof(rect_t), // PhiPos
        sizeof(rect_t), // GLPos
        sizeof(unsigned int), // TexId
        sizeof(unsigned int), // SpriteId
        sizeof(vec2), // Velocity
        sizeof(bool), // Gravity
        sizeof(bool), // RigidCollider
        sizeof(GunType), // GunController
        sizeof(unsigned int), //Amoo
        sizeof(float), // Rotation
        sizeof(float), //Scale
        sizeof(float), //Alpha
        sizeof(BulletKind), //Bullet
        sizeof(float), // Explotion
        sizeof(bool), // Smoke
        sizeof(bool), // Shadow
        sizeof(bool), //Firebarrel
        sizeof(bool), //Jetpack
        sizeof(GranadeComponent) //Granade
    );
}

void systemInit(unsigned int startState) 
{
    //zbug_init(NULL, 0);
    
    viewport = vec3_new((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)SCREEN_SCALE);

    coreInit();
    core_set_2d_scale(viewport.z);
    window_create("Tree World", viewport.x, viewport.y, FULLSCREEN, 1);
    quadVAO = vertex_array_buffer_quad_create();
    //mouse_set_mode(1);

    mouse = vec2_new((viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f);
    cam = vec4_new(0.0f, 0.0f, 1.0f, 0.0f);
    state = oldState = startState;

    screen_color(0.3f, 0.3f, 1.0f, 1.0f);

    randSeed = rand_uint(rand_uint(time(NULL)));
    srand(randSeed);
    printf("Seed: %u\n", randSeed);

    assetsLoad();
    UIassetsLoad();

    modularInit();
    moduleInit();
    archetypesInit();

    blackAndWhite = false;
    initNanoNet();
    treeInit();
}

void treeInit()
{
    menuInit();
    netMenuInit();
    UIeditorInit();
    optionsInit();
    gameInit();
    levelEditorInit();
    spriteEditorInit();
    printf("%u\n entities", entity_count());
}

void treeDirectoryReset()
{
    gameDirectoryReset();
    menuDirectoryReset();
    levelEditorDirectoryReset();
    spriteEditorDirectoryReset();
    UIeditorDirectoryReset();
    optionsDirectoryReset();
    netMenuDirectoryReset();
}

void systemExit()
{
    assetsFree();
    modularExit();
    wxDirectoryFree(&wxDir);
    free(universeFontGet());
    coreExit();
    //zbug_diagnostic();
    //zbug_exit();
    exit(0);
}
