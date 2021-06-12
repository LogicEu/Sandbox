#include <Taxi/Taxi.h>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_SCALE 4

int main(int argc, char** argv)
{
    //zbug_init(NULL, 1);
    coreInit();
    core_set_2d_scale(SCREEN_SCALE);
    window_create("Taxi Textures", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    
    unsigned int shader = shader_load("assets/shaders/texvert.frag", "assets/shaders/texfrag.frag");

    float x, y;
    float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float res[] = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)SCREEN_SCALE};
    float cam[] = {0.0f, 0.0f, 1.0f, 0.0f};
    
    shader_set_uniform(shader, 3, "resolution", &res[0]);
    shader_set_uniform(shader, 4, "camera", &cam[0]);
    shader_set_uniform(shader, 2, "scale_rot", &cam[2]);
    shader_set_uniform(shader, 4, "color", &color[0]);
    
    vertex_array_buffer_quad_create();

    texture_t texture = texture_load("assets/sprites/Ship.png");

    glBindTexture(GL_TEXTURE_2D, texture.id);
    screen_color(0.0f, 0.3f, 1.0f, 1.0f);
    while(window_is_open()) {
        screen_clear();
        mouse_get_position(&x, &y);
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;

        glUniform4f(glGetUniformLocation(shader, "trans"), x, y, texture.width, texture.height);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        screen_refresh();
    }
    coreExit();
    //zbug_diagnostic();
    //zbug_exit();
    return 0;
}