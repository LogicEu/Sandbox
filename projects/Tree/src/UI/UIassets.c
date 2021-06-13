#include "UIcommon.h"

extern vec3 viewport;

wxDirectory wxDir;

static void universeUI()
{
    universeInit(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);

    color_t primary[] = {
        {0.3f, 0.3f, 1.0f, 1.0f}, 
        {0.0f, 0.0f, 1.0f, 1.0f},
        {1.0f, 0.0f, 0.0f, 1.0f}
    };
    color_t secondary[] = {
        {1.0f, 0.3f, 0.3f, 1.0f}, 
        {1.0f, 0.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f, 1.0f}
    };
    color_t terciary[3] = {
        {0.3f, 0.3f, 0.3f, 1.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
        {1.0f, 1.0f, 1.0f, 1.0f}
    };

    universeThemeSet(primary, secondary, terciary);
    universeFontSet(fontLoad("assets/fonts/Pixels.ttf", 32));
    universeSliderSet(
        texture_load("assets/sprites/slider.png"),
        texture_load("assets/sprites/marker.png")
    );
    universeSwitchSet(
        texture_load("assets/sprites/switch_down.png"),
        texture_load("assets/sprites/switch_up.png")
    );
}

static wxGroup mainMenuGroup()
{
    float x = viewport.x * 0.5f / viewport.z;
    wxGroup group = wxGroupCreate();
    
    wxTitle title = wxTitleCreate("TREE", vec3_new(100.0f, 170.0f, 2.5f), color(0.3f, 1.0f, 0.3f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("Main Menu", vec3_new(100.0f, 155.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    
    wxButton button = wxButtonCreate("Play", rect_new(x, 130.0f, 40.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("Level Editor", rect_new(x, 90.0f, 108.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("Quit", rect_new(x, 10.0f, 50.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("UI Editor", rect_new(x, 50.0f, 90.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("Sprite Editor", rect_new(x, 70.0f, 130.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("Options", rect_new(x, 30.0f, 80.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    button = wxButtonCreate("Online", rect_new(x, 110.0f, 60.0f, 15.0f));
    button.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &button, WIDGET_BUTTON);
    
    return group;
}

static wxGroup spriteEditorGroup()
{
    float xscale = viewport.x / viewport.z;
    float yscale = viewport.y / viewport.z;
    
    wxGroup group = wxGroupCreate();
    
    wxTitle title = wxTitleCreate("Sprite Editor Mode", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("R", vec3_new(xscale - 42.0f, 4.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("G", vec3_new(xscale - 32.0f, 4.0f, 0.8f), color(0.0f, 1.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("B", vec3_new(xscale - 22.0f, 4.0f, 0.8f), color(0.0f, 0.0f, 1.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("A", vec3_new(xscale - 12.0f, 4.0f, 0.8f), color(0.5f, 0.5f, 0.5f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    
    wxSlider slider = wxSliderCreate(vec2_new(xscale - 40.0f, 32.0f), 1.0f, true);
    wxGroupPush(&group, &slider, WIDGET_SLIDER);
    slider = wxSliderCreate(vec2_new(xscale - 30.0f, 32.0f), 1.0f, true);
    wxGroupPush(&group, &slider, WIDGET_SLIDER);
    slider = wxSliderCreate(vec2_new(xscale - 20.0f, 32.0f), 1.0f, true);
    wxGroupPush(&group, &slider, WIDGET_SLIDER);
    slider = wxSliderCreate(vec2_new(xscale - 10.0f, 32.0f), 1.0f, true);
    wxGroupPush(&group, &slider, WIDGET_SLIDER);
    
    wxRect rect = wxRectCreate(rect_new(xscale - 32.0f, yscale - 16.0f, 16.0f, 16.0f), color(0.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 16.0f, yscale - 16.0f, 16.0f, 16.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 32.0f, yscale - 32.0f, 16.0f, 16.0f), color(0.0f, 1.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 16.0f, yscale - 32.0f, 16.0f, 16.0f), color(0.0f, 0.0f, 1.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 32.0f, yscale - 48.0f, 16.0f, 16.0f), color(1.0f, 1.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 16.0f, yscale - 48.0f, 16.0f, 16.0f), color(0.0f, 1.0f, 1.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 32.0f, yscale - 64.0f, 16.0f, 16.0f), color(1.0f, 0.0f, 1.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(xscale - 16.0f, yscale - 64.0f, 16.0f, 16.0f), color(1.0f, 1.0f, 1.0f, 1.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    rect = wxRectCreate(rect_new(16.0f, 16.0f, 16.0f, 16.0f), unicolor(0.0f));
    wxGroupPush(&group, &rect, WIDGET_RECT);
    
    return group;
}

static wxGroup optionsGroup()
{
    wxGroup group = wxGroupCreate();
    wxTitle title = wxTitleCreate("Options", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("Mouse", vec3_new(85.0f, 110.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("Fullscreen", vec3_new(75.0f, 70.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);

    wxSwitch sw = wxSwitchCreate(vec2_new(160.0f, 115.0f), 1.0f);
    wxGroupPush(&group, &sw, WIDGET_SWITCH);
    sw = wxSwitchCreate(vec2_new(160.0f, 75.0f), 1.0f);
    wxGroupPush(&group, &sw, WIDGET_SWITCH);
    return group;
}

static wxGroup netMenuGroup()
{
    float xscale = viewport.x / viewport.z;
    float yscale = viewport.y / viewport.z;

    wxGroup group = wxGroupCreate();
    wxTitle title = wxTitleCreate("Online", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("Enter Server's IP:", vec3_new(80.0f, yscale * 0.5f + 48.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    title = wxTitleCreate("Enter User Name:", vec3_new(80.0f, yscale * 0.5f - 16.0f, 0.8f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);

    wxField field = wxFieldCreate(rect_new(xscale * 0.5f, yscale * 0.5f + 32.0f, 154.0f, 16.0f), 15);
    field.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &field, WIDGET_FIELD);
    field = wxFieldCreate(rect_new(xscale * 0.5f, yscale * 0.5f - 32.0f, 154.0f, 16.0f), 15);
    field.text_offset = vec3_new(2.0f, 2.0f, 2.5f);
    wxGroupPush(&group, &field, WIDGET_FIELD);
    return group;
}

static wxDirectory UIassetsDirectory()
{
    wxDirectory dir = wxDirectoryCreate();
    wxGroup group;
    wxTitle title;

    group = mainMenuGroup();
    wxDirectoryPushGroup(&dir, &group);

    group = wxGroupCreate();
    title = wxTitleCreate("Game Mode", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    wxDirectoryPushGroup(&dir, &group);

    group = wxGroupCreate();
    title = wxTitleCreate("Level Editor Mode", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    wxDirectoryPushGroup(&dir, &group);

    group = wxGroupCreate();
    title = wxTitleCreate("UI Editor Mode", vec3_new(4.0f, 185.0f, 1.0f), color(1.0f, 0.0f, 0.0f, 1.0f));
    wxGroupPush(&group, &title, WIDGET_TITLE);
    wxDirectoryPushGroup(&dir, &group);

    group = spriteEditorGroup();
    wxDirectoryPushGroup(&dir, &group);

    group = optionsGroup();
    wxDirectoryPushGroup(&dir, &group);

    group = netMenuGroup();
    wxDirectoryPushGroup(&dir, &group);

    dir.selected = WX_DIR_MAIN_MENU;
    return dir;
}

void UIassetsLoad()
{
    universeUI();
    //wxDir = wxDirectoryLoad(FILE_MENU_UI_SAVE);
    wxDir = UIassetsDirectory();
}