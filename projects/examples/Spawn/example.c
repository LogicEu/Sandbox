#include <Taxi/Taxi.h>
#include <Modular/Modular.h>
#include <Fract/Fract.h>

typedef enum {
    COMP_POS,
    COMP_VEL,
    COMP_TEX,
    COMP_COLOR,
} component_enum;

Entity entity_roulette(texture_t t)
{
    unsigned int i = rand() % 4;
    unsigned int x = rand() % (core.width / core.scale);
    unsigned int y = rand() % (core.height / core.scale);
    unsigned int a = (rand() % 66) / 10;
    vec2 pos = {(float)x, (float)y};
    vec2 vel = rad_to_vec2((float)a);
    color_t alpha = {1.0f, 1.0f, 1.0f, 0.5f};
    switch (i) {
        case 0:
            return archetype_entity(0, 2, &pos, &t);
        case 1:
            return archetype_entity(1, 3, &pos, &t, &alpha);
        case 2:
            return archetype_entity(2, 3, &pos, &vel, &t);
        case 3:
            return archetype_entity(3, 4, &pos, &vel, &t, &alpha);
        break;
    }
    return archetype_entity(0, 2, &pos, &t);
}

void system_manage(float delta_time, unsigned int shader)
{
    float c[] = {1.0f, 1.0f, 1.0f, 1.0f};
    for (int i = 0; i < entity_count(); i++) {
        if (entity_has(i, COMP_POS)) {
            vec2* pos = (vec2*)entity_get(i, COMP_POS);
            if (entity_has(i, COMP_VEL)) {
                vec2* vel = (vec2*)entity_get(i, COMP_VEL);
                if (pos->x > core.width / core.scale || pos->x < 0.0f) vel->x *= -1.0f;
                if (pos->y > core.height / core.scale || pos->y < 0.0f) vel->y *= -1.0f;
                *pos = vec2_add(*pos, vec2_mult(*vel, delta_time));
            }
            if (entity_has(i, COMP_TEX)) {
                if (entity_has(i, COMP_COLOR)) shader_set_uniform_float(shader, 4, "color", (float*)entity_get(i, COMP_COLOR));
                else shader_set_uniform_float(shader, 4, "color", &c[0]);
                texture_t* t = (texture_t*)entity_get(i, COMP_TEX);
                glBindTexture(GL_TEXTURE_2D, t->id);
                glUniform4f(glGetUniformLocation(shader, "trans"), pos->x, pos->y, t->width, t->height);
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            }
        }
    }
}

int main(int argc, char** argv)
{
    //zbug_init(NULL, 0);
    initCore();
    core.scale = 12; 
    window_create("ECS", 800, 600, 100, 100, false);
    srand(rand_uint((unsigned int)time_start_timer()));

    float color[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float background[] = {0.0f, 0.3f, 1.0f, 1.0f};
    float res[] = {(float)core.width, (float)core.height, (float)core.scale};
    float cam[] = {0.0f, 0.0f, 1.0f, 0.0f};

    vertex_array_buffer_quad_create();
    unsigned int shader = shader_load("assets/shaders/texvert.frag", "assets/shaders/texfrag.frag");
    glUseProgram(shader);
    shader_set_uniform_float(shader, 3, "resolution", &res[0]);
    shader_set_uniform_float(shader, 4, "camera", &cam[0]);
    shader_set_uniform_float(shader, 2, "scale_rot", &cam[2]);
    shader_set_uniform_float(shader, 4, "color", &color[0]);
    texture_t t = texture_load("assets/sprites/Ship.png");
    glBindTexture(GL_TEXTURE_2D, t.id);
    
    vec2 mouse = {0.0f, 0.0f};

    ModularInit(4, sizeof(vec2), sizeof(vec2), sizeof(texture_t), sizeof(color_t));
    archetype_create(2, COMP_POS, COMP_TEX);
    archetype_create(3, COMP_POS, COMP_TEX, COMP_COLOR);
    archetype_create(3, COMP_POS, COMP_VEL, COMP_TEX);
    archetype_create(4, COMP_POS, COMP_VEL, COMP_TEX, COMP_COLOR);
    Entity e = archetype_entity(0, 2, &mouse, &t);

    float last_time = time_start_timer();
    screen_color(&background[0]);
    while(window_is_open()) {
        screen_clear();
        float delta_time = time_get_delta_time(&last_time);
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;

        mouse_get_position(&mouse.x, &mouse.y);
        entity_set(e, COMP_POS, &mouse);

        if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            archetype_entity(0, 2, &mouse, &t);
        }
        if (keyboard_pressed(GLFW_KEY_SPACE)) {
            entity_destroy(entity_count() - (entity_count() != 0));
        }

        system_manage(delta_time, shader);
        screen_refresh();
    }

    exitCore();
    ModularExit();
    //zbug_diagnostic();
    //zbug_exit();
    return 0;
}