#include "Tree.h"
#include <pthread.h>

extern vec2 mouse;
extern vec4 cam;
extern unsigned int randSeed;

static bool finished = false;

static void loadGame()
{   
    modularInit();
    moduleInit();
    archetypesInit();

    menuInit();
    editorInit();
    gameInit();
}

static void* loaderLoad(void* value)
{
    loadGame();
    finished = true;
    return NULL;
}

static void loaderDraw()
{
    static color_t red = {1.0f, 0.0f, 0.0f, 1.0f};
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};

    screen_clear();
    mouse_get_position(&mouse.x, &mouse.y);

    glUseProgram(assetsGetShader(SHADER_TEXTURE));
    drawTextureColor(*assetsGetTexture(TEXTURE_MOUSE_CURSOR), vec2_new(mouse.x + 4.0f, mouse.y - 4.0f), unicolor(1.0f));
    drawParallax(cam);
    drawSetCamera(&cam.x);

    glUseProgram(assetsGetShader(SHADER_FONT));
    drawStringCentered("LOADING . . .", 
                assetsGetFont(FONT_1), 
                &red.r,
                center.x - 84.0f, 
                center.y - 32.0f, 
                0.75f, 
                0.0f
    );

    screen_refresh();
}

void loaderStep()
{
    pthread_t thread;
    pthread_create(&thread, NULL, loaderLoad, NULL);
    while (!finished) {
        loaderDraw();
    }
    pthread_join(thread, NULL);
    systemSetState(STATE_MENU);
}