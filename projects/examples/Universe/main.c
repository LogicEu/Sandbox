#include <Universe/Universe.h>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600
#define SCR_SCALE 1

int main(void)
{
    coreInit();
    window_create("Universe Test", SCR_WIDTH, SCR_HEIGHT, 0, 1);
    //universeTheme(primary, secondary, terciary);
    universeInit(SCR_WIDTH, SCR_HEIGHT, SCR_SCALE);

    Font* font = fontLoad("assets/fonts/Emulogic.ttf", 36);

    universeFontSet(font);
    universeFrameSet(texture_load("assets/sprites/frame16.png"));
    universeSliderSet(
        texture_load("assets/sprites/slider.png"),
        texture_load("assets/sprites/marker.png")
    );
    universeSwitchSet(
        texture_load("assets/sprites/switch_down.png"),
        texture_load("assets/sprites/switch_up.png")
    );

    Icon icon = iconCreate(texture_load("assets/sprites/Ship.png"), vec2_new(80.0f, 80.0f), 4.0f, 0.0f);
    wxButton button = wxButtonCreate("Hello Yow!", rect_new(SCR_WIDTH / 2, SCR_HEIGHT / 2, 250.0f, 40.0f));
    wxSwitch sw = wxSwitchCreate(vec2_new(400.0f, 400.0f), 4.0f);
    wxSlider slider = wxSliderCreate(vec2_new(300.0f, 400.0f), 4.0f, 0);
    wxField field = wxFieldCreate(rect_new(SCR_WIDTH / 2, SCR_HEIGHT / 2 - 100.0f, 250.0f, 40.0f), 64);
    wxFrame frame = wxFrameCreate(vec2_new(100.0, 20.0), 4.0f);
    wxIcon wx_icon = wxIconCreate(texture_load("assets/sprites/medkit.png"), vec2_new(80.0f, 300.0f), 4.0f);

    wxGroup group = wxGroupCreate();
    wxDirectory dir = wxDirectoryCreate();
    wxDirectoryPushGroup(&dir, &group);
    wxDirectoryPushWidget(&dir, &button, WIDGET_BUTTON);
    wxDirectoryPushWidget(&dir, &sw, WIDGET_SWITCH);
    wxDirectoryPushWidget(&dir, &slider, WIDGET_SLIDER);
    wxDirectoryPushWidget(&dir, &field, WIDGET_FIELD);
    wxDirectoryPushWidget(&dir, &frame, WIDGET_FRAME);
    wxDirectoryPushWidget(&dir, &wx_icon, WIDGET_WX_ICON);

    vec2 mouse;
    float g = 0.3f;

    while(window_is_open()) {
        screen_color(slider.lerp, g, 1.0f, 1.0f);
        screen_clear();
        mouse_get_position(&mouse.x, &mouse.y);
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;

        bool mousePressed = mouse_pressed(GLFW_MOUSE_BUTTON_LEFT);
        bool mouseDown = mouse_down(GLFW_MOUSE_BUTTON_LEFT);

        wxDirectoryUpdate(&dir, mouse, mousePressed, mouseDown);
        wxDirectoryDraw(&dir);

        iconDraw(&icon);
        fontDrawText(font, "Universe Test", 200.0f, 570.0f, 0.5f, vec3_new(0.3, 0.7f, 0.9f));

        if(keyboard_down(GLFW_KEY_LEFT_CONTROL)) {
            if (keyboard_down(GLFW_KEY_D)) {
                button.rect.w += 0.2f;
            }
            if (keyboard_down(GLFW_KEY_A)) {
                button.rect.w -= 0.2f;
            }
            if (keyboard_down(GLFW_KEY_W)) {
                button.rect.h += 0.2f;
            }
            if (keyboard_down(GLFW_KEY_S)) {
                button.rect.h -= 0.2f;
            }
            if (keyboard_down(GLFW_KEY_P)) {
                icon.rotation += 0.1f;
            }
            if (keyboard_down(GLFW_KEY_O)) {
                icon.rotation -= 0.1f;
            }
            if (keyboard_pressed(GLFW_KEY_Z)) {
                button.rect.w *= 1.1f;
                button.rect.h *= 1.1f;
            }  
            if (keyboard_pressed(GLFW_KEY_X)) {
                button.rect.w *= 0.9f;
                button.rect.h *= 0.9f; 
            }  
        } 

        if (button.state == WIDGET_SELECTED) printf("%s\n", field.text);
        if (sw.activated) g = 1.0f;
        else g = 0.3f;

        screen_refresh();
    }
    coreExit();
    free(font);
    wxDirectoryFree(&dir);
    return 0;
}
