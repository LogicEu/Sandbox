#include "Tree.h"

static unsigned int state;
static unsigned int oldState;

unsigned int randSeed;
unsigned int quadVAO;

extern unsigned int hp;
extern wxDirectory wxDir;

vec2 mouse;
vec3 viewport;
vec4 cam;

static void systemSnapshot()
{
    unsigned char img[SCREEN_WIDTH * 2 * SCREEN_HEIGHT * 2 * 4];
    glReadPixels(0, 0, SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, GL_RGBA, GL_UNSIGNED_BYTE, &img[0]);
    img_file_write("test.png", &img[0], SCREEN_WIDTH * 2, SCREEN_HEIGHT * 2, 4);
}

static void systemDraw()
{
    if (state == STATE_PAUSE || !hp) framebuffer_bind(assetsGetFramebuffer(FRAMEBUFFER_BLACK_AND_WHITE)->id);
    
    //glUseProgram(assetsGetShader(SHADER_TEXTURE));
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindVertexArray(quadVAO);
    drawParallax(cam);
    drawSetCamera(&cam.x);
    if (state == STATE_PLAY || state == STATE_LEVEL_EDITOR || state == STATE_PAUSE) drawComponents();
}

static void systemPause()
{
    static color_t red = {1.0f, 0.0f, 0.0f, 1.0f};
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};

    if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)   ||
        keyboard_pressed(GLFW_KEY_SPACE)        || 
        keyboard_pressed(GLFW_KEY_BACKSPACE)    || 
        keyboard_pressed(GLFW_KEY_N)) {
        state = oldState;
    }
    if (keyboard_pressed(GLFW_KEY_ESCAPE) || keyboard_pressed(GLFW_KEY_Y)) {
        if (oldState == STATE_PLAY || oldState == STATE_LEVEL_EDITOR) systemSetState(STATE_MENU);
        else systemExit();
    }
    if (keyboard_pressed(GLFW_KEY_M)) {
        systemSetState(STATE_MENU);
    }

    drawFramebuffer();
    
    glUseProgram(assetsGetShader(SHADER_FONT));
    drawStringCentered(
        "GAME IS PAUSED", 
        assetsGetFont(FONT_1), 
        &red.r,
        center.x - 84.0f, 
        center.y - 32.0f, 
        0.75f, 
        0.0f
    );

    drawStringCentered(
        "Do you want to quit?", 
        assetsGetFont(FONT_1), 
        &red.r,
        center.x - 174.0f, 
        center.y - 48.0f, 
        0.5f, 
        0.0f
    );
}

static void systemInput()
{
    if (core_need_update()) {
        core_update();
    }

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
    }
}

void systemStep(float deltaTime)
{
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
            systemPause();
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
    //loaderStep();

    UIassetsLoad();

    modularInit();
    moduleInit();
    archetypesInit();

    randSeed = rand_uint(rand_uint(time(NULL)));
    srand(randSeed);
    printf("Seed: %u\n", randSeed);

    menuInit();
    optionsInit();
    levelEditorInit();
    spriteEditorInit();
    gameInit();
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