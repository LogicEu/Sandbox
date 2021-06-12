#include "../game.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 600
#define SCREEN_SCALE 4
#define SCALE_WIDTH SCREEN_WIDTH / SCREEN_SCALE
#define SCALE_HEIGHT SCREEN_HEIGHT / SCREEN_SCALE

int main()
{
    initFMOD();
    GLFWwindow* window = initOpenGL("The Hole", SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!window) return -1;
    srand((unsigned int)glfwGetTime());

    float res[] = {(float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, (float)SCREEN_SCALE};
    float cam[] = {0.0f, 0.0f, 1.0f, 0.0f};
    
    float bck[] = {0.5f, 0.5f, 1.0f, 1.0f};
    float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    float red[] = {1.0f, 0.0f, 0.0f, 1.0f};
    float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float blue[] = {0.0f, 0.0f, 1.0f, 1.0f};
    float cloud_color[] = {1.0f, 1.0f, 1.0f, 0.4f};

    quad_buffer_create();
    unsigned int shader_texture, shader_font;
    shaders_load(&shader_texture, &shader_font, res, cam, white);

    texture_t* textures = textures_load();
    font_t* font = font_load("assets/fonts/arcadeclassic.ttf", 16);
    sound_t* sounds = sounds_load();
    
    unsigned int ground_size, can_jump = 0;
    const float start_x = 16.0f, start_y = SCALE_HEIGHT - 48.0f;
    entity_t player = {TEXTURE_PLAYER, start_x, start_y, textures[TEXTURE_PLAYER].width, textures[TEXTURE_PLAYER].height};
    entity_t* ground = ground_init(SCALE_WIDTH, SCALE_HEIGHT, &ground_size);
    entity_t* bats = bats_init(SCALE_WIDTH);
    entity_t cloud = entity(TEXTURE_CLOUD, SCALE_WIDTH / 2, SCALE_HEIGHT - 24, 96.0f, 48.0f);

    const float gravity = 10.0f, hspeed = 70.0f, jump_speed = 200.0f, dash_speed = 200.0f;
    float xspeed = 0.0f, yspeed = 0.0f;
    float last_time = (float)glfwGetTime(), current_time, delta_time, animation_timer = 1.0f, game_over_timer = 3.0f;
    
    const char* highscore_path = "assets/.highscore.txt";
    float highscore = highscore_load(highscore_path);
    float mts = 0.0f;
    char str_ui[32] = "0.0 mts";

    sound_play(sounds[SOUND_FUNK]);
    unsigned int game_state = STATE_PLAY;

    while(!glfwWindowShouldClose(window)) {
        current_time = glfwGetTime();
        delta_time = current_time - last_time;
        last_time = current_time;
        
        glClearColor(bck[0], bck[1], bck[2], bck[3]);
        glClear(GL_COLOR_BUFFER_BIT);
        
        if(keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_pressed(GLFW_KEY_M)) sound_stop();
        if (keyboard_pressed(GLFW_KEY_R)) {
            player.x = start_x;
            player.y = start_y;
            xspeed = 0.0f;
            yspeed = 0.0f;
            cam[1] = 0.0f;
        }

        // UPDATE
        if (game_state == STATE_PLAY) {
            xspeed = lerpf(xspeed, 0.0f, 0.2f);
            if ((keyboard_down(GLFW_KEY_D) || keyboard_down(GLFW_KEY_RIGHT))) { 
                if (!place_meeting_entity_array(ground, ground_size, player, (hspeed + xspeed) * delta_time, 1.0f)) {
                    if (player.x > SCALE_WIDTH + 4.0f) player.x = -4.0f;
                    player.x += hspeed * delta_time;
                } else {
                    yspeed = clampf(yspeed, -15.0f, 400.0f);
                    if ((keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP)) &&
                        !place_meeting_entity_array(ground, ground_size, player, -dash_speed * delta_time, jump_speed * delta_time)) {
                        sound_play(sounds[SOUND_JUMP]);
                        yspeed = jump_speed;
                        xspeed = -dash_speed;
                    }
                }
                player.w = (float)textures[TEXTURE_PLAYER].width;
            }
            if ((keyboard_down(GLFW_KEY_A) || keyboard_down(GLFW_KEY_LEFT))) {
                if (!place_meeting_entity_array(ground, ground_size, player, (-hspeed + xspeed) * delta_time, 1.0f)) {
                    if (player.x < -4.0f) player.x = SCALE_WIDTH + 4.0f;
                    player.x -= hspeed * delta_time;
                } else {
                    yspeed = clampf(yspeed, -15.0f, 400.0f);
                    if ((keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP)) &&
                        !place_meeting_entity_array(ground, ground_size, player, dash_speed * delta_time, jump_speed * delta_time)) {
                        sound_play(sounds[SOUND_JUMP]);
                        yspeed = jump_speed;
                        xspeed = dash_speed;
                    }
                }
                player.w = -(float)textures[TEXTURE_PLAYER].width;
            }

            if ((keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP)) && can_jump &&
                !place_meeting_entity_array(ground, ground_size, player, 0.0f, jump_speed * delta_time)) {
                sound_play(sounds[SOUND_JUMP]);
                yspeed = jump_speed;
            }

            if (!place_meeting_entity_array(ground, ground_size, player, 0.0f, yspeed * delta_time)) {
                yspeed -= gravity;
                can_jump = 0;
            } else {
                yspeed = 0.0f;
                can_jump = 1;
            }
            yspeed = clampf(yspeed, -400.0f, 400.0f);
            player.y += yspeed * delta_time;

            player.x += xspeed * delta_time;
            if (yspeed < 0.0f && player.y - cam[1] < SCALE_HEIGHT / 2) {
                cam[1] += yspeed * delta_time;
            }

            if (place_meeting_entity_array(bats, SIZE_OF_BATS, player, 0.0f, 0.0f)) {
                sound_stop();
                sound_play(sounds[SOUND_GAME_OVER]);
                game_state = STATE_GAME_OVER;
                if (mts > highscore) {
                    highscore = mts;
                    highscore_save(highscore_path, str_ui);
                }
            }

            float dif = SCALE_HEIGHT / 2 - player.y;
            if (dif > mts) {
                mts = dif / 4.0f;
                if (bck[0] > 0.0f) {
                    bck[0] -= dif * delta_time * 0.0001f;
                    bck[1] -= dif * delta_time * 0.0001f; 
                } else if (bck[2] > 0.3f) bck[2] -= dif * delta_time * 0.0001f;
            }
        }

        // DRAW 
        glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_PLAYER].id);
        shader_set_uniform(shader_texture, 4, "color", white);
        shader_set_uniform(shader_texture, 4, "camera", cam);
        shader_set_uniform(shader_texture, 4, "trans", &player.x);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        for (int i = 0; i < ground_size; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[ground[i].id].id);
            shader_set_uniform(shader_texture, 4, "trans", &ground[i].x);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            if (game_state == STATE_GAME_OVER) continue;
            float d = ground[i].y - player.y;
            if (d > SCALE_HEIGHT / 2 + 8.0f) {
                if (ground[i].id < 5) {
                    ground[i].id = TEXTURE_GROUND;
                } else if (ground[i].id == TEXTURE_CORNER_RIGHT) {
                    ground[i].id = TEXTURE_BLOCK_RIGHT;
                }
                else if (ground[i].id == TEXTURE_CORNER_LEFT) {
                    ground[i].id = TEXTURE_BLOCK_LEFT;
                }
                ground[i].y -= 8.0f * 24;
                if (i >= 192) ground[i].x = (float)(SCALE_WIDTH / 2 - 18 + rand() % 36);
            }
        }

        for (int i = 0; i < SIZE_OF_BATS; i++) {
            glBindTexture(GL_TEXTURE_2D, textures[bats[i].id].id);
            shader_set_uniform(shader_texture, 4, "trans", &bats[i].x);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            if (animation_timer < 0.0f) {
                if (bats[i].id < TEXTURE_BAT5) bats[i].id ++;
                else bats[i].id = TEXTURE_BAT0;
            } 

            if (bats[i].w > 0.0f) {
                if (!place_meeting_entity_array(ground, ground_size, bats[i], hspeed * delta_time, 0.0f)) {
                    bats[i].x += hspeed * delta_time;
                } else bats[i].w = -bats[i].w;
            } else {
                if (!place_meeting_entity_array(ground, ground_size, bats[i], -hspeed * delta_time, 0.0f)) {
                    bats[i].x -= hspeed * delta_time;
                } else bats[i].w = -bats[i].w;
            }

            if (game_state == STATE_GAME_OVER) continue;
            float d = bats[i].y - player.y;
            if (d > SCALE_HEIGHT / 2 + 8.0f) {
                bats[i].y -= 8.0f * 24;
                bats[i].x = (float)(SCALE_WIDTH / 2 - 18 + rand() % 36);
            }
        }
        if (animation_timer < 0.0f) animation_timer = 1.0f;
        else animation_timer -= delta_time * 15.0f;

        //UI
        glUseProgram(shader_font);
        font_draw_string(shader_font, "The  Hole", font, black, 29.0f, SCALE_HEIGHT - 17.0f, 0.75f, 0.0f);
        font_draw_string(shader_font, "The  Hole", font, red, 28.0f, SCALE_HEIGHT - 16.0f, 0.75f, 0.0f);
        sprintf(str_ui, "%u mts", (unsigned int)mts);
        font_draw_string(shader_font, str_ui, font, red, 28.0f, SCALE_HEIGHT - 24.0f, 0.75, 0.0f);
        sprintf(str_ui, "%u mts", (unsigned int)highscore);
        font_draw_string(shader_font, str_ui, font, blue, 28.0f, SCALE_HEIGHT - 32.0f, 0.75, 0.0f);
        if (game_state == STATE_GAME_OVER) {
            font_draw_string(shader_font, "GAME  OVER", font, red, SCALE_WIDTH / 2 - 32.0f, SCALE_HEIGHT / 2, 1.0f, 0.0f);
            game_over_timer -= delta_time;
            if (game_over_timer < 0.0f) {
                mts = 0.0f;
                xspeed = 0.0f;
                yspeed = 0.0f;
                player.x = start_x;
                player.y = start_y;
                cam[1] = 0.0f;
                sound_play(sounds[SOUND_FUNK]);
                free(bats);
                free(ground);
                bats = bats_init(SCALE_WIDTH);
                ground = ground_init(SCALE_WIDTH, SCALE_HEIGHT, &ground_size);
                bck[0] = 0.5f;
                bck[1] = 0.5f;
                bck[2] = 1.0f;
                game_state = STATE_PLAY;
                game_over_timer = 3.0f;
            }
        }

        glUseProgram(shader_texture);
        glBindTexture(GL_TEXTURE_2D, textures[TEXTURE_CLOUD].id);
        shader_set_uniform(shader_texture, 4, "trans", &cloud.x);
        shader_set_uniform(shader_texture, 4, "color", cloud_color);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteProgram(shader_texture);
    glDeleteProgram(shader_font);
    glfwTerminate();

    sounds_free(sounds);
    free(ground);
    free(bats);
    free(textures);
    free(font);
    return 0;
}