#include "Tree.h"
#include "UI/UIcommon.h"

#define FILE_ECS_MODULE "assets/data/module.bin"
#define SCALE_MULT 4.0f

extern vec4 cam;
extern vec2 mouse;
extern wxDirectory wxDir;
extern Entity player;

static vec2 cursor;
static Entity selected;

static wxGroup* group;

typedef enum {
    WX_LE_TITLE_MAIN,
    WX_LE_TITLE_ZOOM,
    WX_LE_BUTTON_MENU,
    WX_LE_SLIDER_ZOOM
} wxLevelEditorEnum;

typedef enum {
    ARCH_SWITCH_TILE_GRASS,
    ARCH_SWITCH_TILE_DIRT,
    ARCH_SWITCH_GUN,
    ARCH_SWITCH_SHOTGUN,
    ARCH_SWITCH_RIFLE,
    ARCH_SWITCH_MACHINEGUN,
    ARCH_SWITCH_FLAMETHROWER,
    ARCH_SWITCH_BAZOOKA,
    ARCH_SWITCH_FIREBARREL,
    ARCH_SWITCH_JETPACK,
    ARCH_SWITCH_GRANADE
} archSwitchEnum;

static archSwitchEnum selectedIndex;

static texture_t* textureGet()
{
    switch (selectedIndex) {
        case ARCH_SWITCH_TILE_GRASS:
            return assetsGetTexture(TEXTURE_TILE_GRASS);
        case ARCH_SWITCH_TILE_DIRT:
            return assetsGetTexture(TEXTURE_TILE_DIRT);
        case ARCH_SWITCH_GUN:
            return assetsGetTexture(TEXTURE_GUN);
        case ARCH_SWITCH_SHOTGUN:
            return assetsGetTexture(TEXTURE_SHOTGUN);
        case ARCH_SWITCH_RIFLE:
            return assetsGetTexture(TEXTURE_RIFLE);
        case ARCH_SWITCH_MACHINEGUN:
            return assetsGetTexture(TEXTURE_MACHINEGUN);
        case ARCH_SWITCH_FLAMETHROWER:
            return assetsGetTexture(TEXTURE_FLAMETHROWER);
        case ARCH_SWITCH_BAZOOKA:
            return assetsGetTexture(TEXTURE_BAZOOKA);
        case ARCH_SWITCH_FIREBARREL:
            return assetsGetTexture(TEXTURE_FIREBARREL);
        case ARCH_SWITCH_JETPACK:
            return assetsGetTexture(TEXTURE_JETPACK);
        case ARCH_SWITCH_GRANADE:
            return assetsGetTexture(TEXTURE_GRANADE);
        break;
    }
    return NULL;
}

static Entity entityCreate()
{
    switch (selectedIndex) {
        case ARCH_SWITCH_TILE_GRASS:
            return archetypeTerrainTile(TEXTURE_TILE_GRASS, cursor);
        case ARCH_SWITCH_TILE_DIRT:
            return archetypeTerrainTile(TEXTURE_TILE_DIRT, cursor);
        case ARCH_SWITCH_GUN:
            return gunCreate(cursor, GUN_KIND_GUN);
        case ARCH_SWITCH_SHOTGUN:
            return gunCreate(cursor, GUN_KIND_SHOTGUN);
        case ARCH_SWITCH_RIFLE:
            return gunCreate(cursor, GUN_KIND_RIFLE);
        case ARCH_SWITCH_MACHINEGUN:
            return gunCreate(cursor, GUN_KIND_MACHINEGUN);
        case ARCH_SWITCH_FLAMETHROWER:
            return gunCreate(cursor, GUN_KIND_FLAMETHROWER);
        case ARCH_SWITCH_BAZOOKA:
            return gunCreate(cursor, GUN_KIND_BAZOOKA);
        case ARCH_SWITCH_FIREBARREL:
            return archetypeFirebarrel(cursor);
        case ARCH_SWITCH_JETPACK:
            return archetypeJetpackController(cursor);
        case ARCH_SWITCH_GRANADE:
            return archetypeGranade(cursor);
        break;
    }
    return 0;
}

static void getMouseInput()
{
    const float tileSize = 32.0f;
    vec2 center = {(viewport.x / viewport.z) * 0.5f, (viewport.y / viewport.z) * 0.5f};

    bool mousePressed = mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
    wxGroupUpdate(group, mouse, mousePressed, mouse_down(GLFW_MOUSE_BUTTON_LEFT));

    mouse = vec2_add(vec2_add(vec2_div(vec2_sub(mouse, center), cam.z), center), *(vec2*)&cam);
    cursor = vec2_add(vec2_mult(to_vec2(to_ivec2(vec2_div(mouse, tileSize))), tileSize), univec2(tileSize * 0.5f));

    wxSlider* slider = group->widgets[WX_LE_SLIDER_ZOOM].widget;
    if (slider->selected) {
        cam.z = 0.6f + slider->lerp * SCALE_MULT;
        return;
    }
    wxButton* button = group->widgets[WX_LE_BUTTON_MENU].widget;
    if (button->state == WIDGET_SELECTED) {
        systemSetState(STATE_MENU);
        return;
    }

    Entity hover = 0;
    for (Entity e = 1; e < entity_count(); e++) {
        rect_t* r = (rect_t*)entity_get(e, COMPONENT_PHI_RECT);
        if (!r) continue;
        if (point_meeting(cursor, *r)) {
            hover = e;
            break;
        }
    }

    if (hover) {
        glUseProgram(assetsGetShader(SHADER_TEXTURE));
        drawRect(*(rect_t*)entity_get(hover, COMPONENT_GL_RECT), unicolor(0.5f));
        if (mousePressed) {
            selected = hover;
        }
    } else if (mousePressed) {
        entityCreate();
    }

    if (selected) {
        rect_t* texRect = (rect_t*)entity_get(selected, COMPONENT_GL_RECT);
        rect_t r = {cursor.x, cursor.y, texRect->w, texRect->h};
        entity_set(selected, COMPONENT_PHI_RECT, &r);
        entity_set(selected, COMPONENT_GL_RECT, &r);
        if (keyboard_pressed(GLFW_KEY_BACKSPACE)) {
            entity_destroy(selected);
            selected = 0;
        }
        if (mouse_released(GLFW_MOUSE_BUTTON_LEFT)) selected = 0;
    }
}

static void getKeyboardInput(float deltaTime)
{
    static const float camSpeed = 50.0f;

    if (keyboard_pressed(GLFW_KEY_ESCAPE)) {
        systemSetState(STATE_MENU);
        return;
    }
    if (keyboard_pressed(GLFW_KEY_L)) {
        module_destroy(module_current());
        module_load(FILE_ECS_MODULE);
    }
    if (keyboard_down(KEY_MOD) && keyboard_pressed(GLFW_KEY_P)) {
        entity_destroy(player);
        module_save(FILE_ECS_MODULE, module_current());
        player = archetypePlayer();
        printf("Without character!\n");
    }
    if (keyboard_pressed(GLFW_KEY_P)) {
        module_save(FILE_ECS_MODULE, module_current());
    }
    if (keyboard_pressed(GLFW_KEY_R)) {
        levelReset();
    }
    if (keyboard_pressed(GLFW_KEY_C)) {
        if (selectedIndex < ARCH_SWITCH_GRANADE) selectedIndex ++;
        else selectedIndex = ARCH_SWITCH_TILE_GRASS;
    }
    if (keyboard_pressed(GLFW_KEY_V)) {
        if (selectedIndex > ARCH_SWITCH_TILE_GRASS) selectedIndex --;
        else selectedIndex = ARCH_SWITCH_GRANADE;
    }
    if (keyboard_pressed(GLFW_KEY_T)) {
        terrainReduce();
    }
    if (keyboard_pressed(GLFW_KEY_Y)) {
        terrainRecalcTextures();
    }
    if (keyboard_pressed(GLFW_KEY_U)) {
        terraingGenRand(32, 32);
    }
    if (keyboard_down(GLFW_KEY_D)) cam.x += deltaTime * camSpeed;
    if (keyboard_down(GLFW_KEY_A)) cam.x -= deltaTime * camSpeed;
    if (keyboard_down(GLFW_KEY_W)) cam.y += deltaTime * camSpeed;
    if (keyboard_down(GLFW_KEY_S)) cam.y -= deltaTime * camSpeed;
    if (keyboard_down(GLFW_KEY_Z)) cam.z += deltaTime;
    if (keyboard_down(GLFW_KEY_X)) cam.z -= deltaTime;
}

static void levelEditorDraw()
{
    static color_t white = {1.0f, 1.0f, 1.0f, 1.0f};
    static color_t alpha = {1.0f, 1.0f, 1.0f, 0.5f};
    static float camera[] = {0.0f, 0.0f, 1.0f, 0.0f};
    vec2 pos = {(viewport.x / viewport.z) * 0.5f + 90.0f, (viewport.y / viewport.z) - 24.0f};

    wxGroupDraw(group);

    glBindVertexArray(quadVAO);
    glUseProgram(assetsGetShader(SHADER_TEXTURE));
    if (!selected) drawTextureColor(*textureGet(), cursor, alpha);
    drawTextureColor(*assetsGetTexture(TEXTURE_MOUSE_CURSOR), vec2_new(mouse.x + 4.0f, mouse.y - 4.0f), unicolor(1.0f));
    shader_set_uniform(assetsGetShader(SHADER_TEXTURE), 4, "camera", &camera[0]);
    drawTextureColor(*textureGet(), pos, white);
}

static void getInput(float deltaTime)
{
    getKeyboardInput(deltaTime);
    getMouseInput();
}

void levelEditorStep(float deltaTime)
{
    getInput(deltaTime);
    levelEditorDraw();
}

void levelEditorDirectoryReset()
{
    group = &wxDir.groups[WX_DIR_LEVEL_EDITOR];
}

void levelReset()
{
    module_destroy(module_current());
    moduleInit();
    player = archetypePlayer();
}

void levelEditorInit()
{
    levelEditorDirectoryReset();
    selectedIndex = ARCH_SWITCH_TILE_GRASS;
    selected = 0;
}