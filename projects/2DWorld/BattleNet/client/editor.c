#include "BNGame.h"

void recalc_blocks(array_t* blocks, const unsigned int id)
{
    const vec2 off = {0.0f, 32.0f};
    entity_t* e = (entity_t*)blocks->data;
    for (entity_t* end = e + blocks->used; e != end; e++) {
        if (place_meeting_entity_array_offset(blocks->data, blocks->used, e, off)) e->id = id + 1;
        else e->id = id;
    }
}

void reduce_blocks(array_t* blocks)
{
    unsigned int count = 0;
    entity_t* e = (entity_t*)blocks->data;
    for (entity_t* end = e + blocks->used; e != end; e++) {
        for (entity_t* ee = e + 1; ee != end; ee++) {
            if (!memcmp(&e->rect, &ee->rect, sizeof(rect_t))) {

            }
        }
        count++;
    }
}

int main(int argc, char** argv)
{
    //zbug_init(NULL, 1);
    coreInit();
    window_create("Battle Net Editor", SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);
    core_set_2d_scale(SCREEN_SCALE);

    vertex_array_buffer_quad_create();
    shader_init();
    font_t* font = font_load("assets/fonts/arcadeclassic.ttf", 48);
    texture_t block = texture_load("assets/sprites/GrassBlock2.png");
    texture_t dirt_block = texture_load("assets/sprites/DirtBlock.png");
    color_t background = {0.5f, 0.6f, 1.0f, 1.0f};

    array_t* blocks = buffer_load_array("assets/maps/bin.txt", sizeof(entity_t));

    vec4 cam = {0.0f, 0.0f, 1.0f, 0.0f};
    vec2 cursor = {0.0f, 0.0f};
    vec2 off_vec = {16.0f, 16.0f};
    vec2 center = {SCREEN_XSCALE / 2, SCREEN_YSCALE / 2};

    const float speed = 50.0f;
    float last_time = time_start_timer(), delta_time;
    screen_color(&background.r);
    while(window_is_open()) {
        delta_time = time_get_delta_time(&last_time);
        screen_clear();
        mouse_get_position(&cursor.x, &cursor.y);
        cursor = vec2_add(vec2_add(vec2_div(vec2_sub(cursor, center), cam.z), center), *(vec2*)&cam);
        cursor = vec2_add(off_vec, vec2_mult(to_vec2(to_ivec2(vec2_div(vec2_add(cursor, *(vec2*)&cam), core.scale * 16))), core.scale * 16));

        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_down(GLFW_KEY_D)) {
            cam.x += delta_time * speed;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_A)) {
            cam.x -= delta_time * speed;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_W)) {
            cam.y += delta_time * speed;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_S)) {
            cam.y -= delta_time * speed;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_Z)) {
            cam.z += delta_time;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_X)) {
            cam.z -= delta_time;
            shader_set_uniform_float(texshader, 4, "camera", &cam.x);
        }
        if (keyboard_pressed(GLFW_KEY_P)) buffer_write_array("assets/maps/bin.txt", blocks);
        if (keyboard_pressed(GLFW_KEY_N)) recalc_blocks(blocks, block.id);

        if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            entity_t e = entity_new(block.id, cursor.x, cursor.y, 32, 32);
            array_push(blocks, &e);
            printf("Size of blocks: %u\n", blocks->used);
        }

        rect_t r = {cursor.x, cursor.y, 32, 32};
        glUseProgram(texshader);
        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 0.5f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), 1.0f, 0.0f);
        glBindTexture(GL_TEXTURE_2D, block.id);
        glUniform4f(glGetUniformLocation(texshader, "trans"), r.x, r.y, r.w, r.h);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        float k = .05f;
        entity_t* e;
        /*e = (entity_t*)blocks->data;
        glUniform4f(glGetUniformLocation(texshader, "color"), 0.0f, 0.0f, 0.0f, 1.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale + k, e->rot);
        for (entity_t* end = e + blocks->used; e != end; e++) {
            glBindTexture(GL_TEXTURE_2D, e->id);
            glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }*/
        
        e = (entity_t*)blocks->data;
        glUniform4f(glGetUniformLocation(texshader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
        glUniform2f(glGetUniformLocation(texshader, "scale_rot"), e->scale, e->rot);
        for (entity_t* end = e + blocks->used; e != end; e++) {
            glBindTexture(GL_TEXTURE_2D, e->id);
            glUniform4f(glGetUniformLocation(texshader, "trans"), e->rect.x, e->rect.y, e->rect.w * 2.0f, e->rect.h * 2.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        screen_refresh();
    }
    array_free(blocks);
    free(blocks);
    free(font);
    coreExit();
    //zbug_diagnostic();
    //zbug_exit();
}
