#include "../Tree.h"

#define FILE_MENU_UI_SAVE "assets/data/menu_style.bin"

typedef enum {
    WX_DIR_MAIN_MENU,
    WX_DIR_GAME,
    WX_DIR_LEVEL_EDITOR,
    WX_DIR_UI_EDITOR,
    WX_DIR_SPRITE_EDITOR,
    WX_DIR_OPTIONS,
    WX_DIR_NET_MENU
} dirEnum;

typedef enum {
    WX_TITLE_GAME,
    WX_TITLE_MAIN_MENU,
    WX_BUTTON_PLAY,
    WX_BUTTON_LEVEL_EDITOR,
    WX_BUTTON_QUIT,
    WX_BUTTON_UI_EDITOR,
    WX_BUTTON_SPRITE_EDITOR,
    WX_BUTTON_OPTIONS,
    WX_BUTTON_ONLINE
} menuUIEnum;

typedef enum {
    WX_GAME_TITLE
} gameUIEnum;

typedef enum {
    WX_LEVEL_EDITOR_TITLE
} levelEditorUIEnum;

extern wxDirectory wxDir;

void mouseDraw();
void cameraBackgroundSlide();
