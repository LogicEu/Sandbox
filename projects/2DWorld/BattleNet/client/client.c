#include "BNGame.h"
#include <math.h>

int main()
{
    //zbug_init("dump/BNClient.bug");
    zbug_init(NULL, 1);

    char host[16];
    uint16_t port;
    read_net_file("assets/host.net", &host[0], &port);
    initNanoNet();
    printf("Host: %s\nPort: %hd\n", host, port);
    
    list_t* peers = list_new(sizeof(packet_t));
    list_t* events = list_new(sizeof(packet_t));
    list_t* objects = list_new(sizeof(packet_t));

    list_t* my_objects = list_new(sizeof(packet_t*));
    list_t* pre_objects = list_new(sizeof(packet_t*));
    list_t* post_objects = list_new(sizeof(packet_t*));
    list_t* static_objects = list_new(sizeof(packet_t*));
    list_t* throw_packets = list_new(sizeof(packet_t*));

    NNetHost* client = net_init_world(host, port, peers, events, objects, my_objects, pre_objects, post_objects, static_objects);
    packet_t* user, *used_obj = NULL, *jetpack = NULL;

    initCore();
    core.scale = SCREEN_SCALE;
    window_create("BattleNet2D", SCREEN_WIDTH, SCREEN_HEIGHT, 100, 100, false);
    vertex_array_buffer_quad_create();
    shader_init();
    font_t* font = font_load("assets/fonts/arcadeclassic.ttf", 48);

    vec2 center = {SCREEN_XSCALE / 2.0f, SCREEN_YSCALE / 2.0f};
    if (client->connected) {
        user = packet_find_by_id(peers, client->id);
        printf("Player ID: %u\n", client->id);
    } else {
        user = packet_new(client->id, STATE_LIVE, center.x - 10.0f, center.y - 20.0f, 1.0f);
        user->hp = 100;
    }

    //KID
    color_t col = color(1.0f, 1.0f, 1.0f, 1.0f);
    array_t* colors = color_array();
    array_t* color_players = load_color_player_sprites(colors);
    array_t* obj_sprites = load_object_sprites();
    array_t* parallax_sprites = load_parallax();

    texture_t* player_texture = sprite_by_id(color_players, client->id, KID_STANDING)->textures;
    entity_t* player = (entity_t*)malloc(sizeof(entity_t));
    *player = entity_new(player_texture->id, center.x - 10.0f, center.y - 20.0f, player_texture->width, player_texture->height);
    texture_t* aim_texture = texture_obj(obj_sprites, SPRITE_AIM);

    array_t* clouds = clouds_init(sprite_obj(obj_sprites, SPRITE_CLOUD), 60);
    array_t* blocks = blocks_load_char_map("assets/maps/bigmap.txt", *texture_obj(obj_sprites, SPRITE_BLOCK), *texture_obj(obj_sprites, SPRITE_DIRT_BLOCK));

    list_t* throw_objects = list_new(sizeof(entity_t));
    list_t* bullets = list_new(sizeof(entity_t));
    list_t* explotions = list_new(sizeof(entity_t));
    list_t* smoke = list_new(sizeof(entity_t));
    list_t* post_smoke = list_new(sizeof(entity_t));
    list_t* shadows = list_new(sizeof(entity_t));
    list_t* blood = list_new(sizeof(entity_t));
    float camera_scale = 1.0f;
    vec2 mouse = center;
    vec2 cam = {0.0f, 0.0f};
    color_t background = {0.5f, 0.6f, 1.0f, 1.0f};

    bool can_double_jump = true;
    bool wall_sliding = false;
    bool can_dash = true;
    bool is_dead = false;
    bool is_grounded = false;

    float camera_alarm = 1.0f;
    float camera_target = 0.5f;
    float smoke_alarm = 1.0f;
    float shadow_alarm = 1.0f;
    float dead_timer = dead_wait;
    float right_dash_timer = double_click;
    float left_dash_timer = double_click;
    float is_dashing = 0.0f;

    char draw_string[7] = "00";
    unsigned int dammage = 0;
    unsigned int received = 0;

    float last_time = time_start_timer();
    while(window_is_open()) {
        screen_color(&background.r);
        screen_clear();
        float delta_time = time_get_delta_time(&last_time);
        //printf("%f\n", delta_time);

        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_pressed(GLFW_KEY_R)) player_respawn(player, user, player_texture);

        camera_alarm -= delta_time * camera_wait;
        smoke_alarm -= delta_time * smoke_wait;
        shadow_alarm -= delta_time * shadow_wait;

        //Camera
        update_cam(&cam, player, camera_scale);
        if (is_dead || (camera_alarm < 0.0f)) {
            camera_target = lerpf(camera_target, 1.0f, 0.005f);
            camera_scale = lerpf(camera_scale, camera_target, 0.005f);
        } else {
            camera_target = lerpf(camera_target, 0.5f, 0.01f);
            camera_scale = lerpf(camera_scale, camera_target, 0.01f);
        }
        
        //Gun & Aim
        mouse_get_position(&mouse.x, &mouse.y);
        mouse = vec2_add(vec2_add(vec2_div(vec2_sub(mouse, center), camera_scale), center), cam);

        //Draw & and update
        draw_parallax(parallax_sprites, cam, center);
        draw_blocks(blocks);
        draw_pre_objects(pre_objects, obj_sprites);
        update_shadows(shadows, delta_time);
        update_smoke(smoke, delta_time);
        update_blood(blood, delta_time);
        update_bullets(bullets, player, color_players, blocks, static_objects, delta_time, events, explotions, obj_sprites, user->id);
        dammage = update_explotions(explotions, player, delta_time);
        update_static_objects(static_objects, explotions, events, my_objects, obj_sprites, user->id);
        
        draw_peers(peers, color_players, user);
        draw_sprite_transformed(texshader, sprite_by_id(color_players, user->id, user->index), player->rect.x, player->rect.y, (float)player_texture->width * user->rot, (float)player_texture->height, 1.0f, player->rot);
        
        //Post Draw
        draw_post_objects(post_objects, obj_sprites);
        update_post_smoke(post_smoke, delta_time);
        update_clouds(clouds, delta_time);
        
        //UI
        draw_texture_transformed(texshader, aim_texture, col, mouse.x, mouse.y, (float)aim_texture->width, (float)aim_texture->height, 1.0f, 0.0f);
        float cam[] = {0.0f, 0.0f, 1.0f, 0.0f};
        shader_set_uniform_float(texshader, 4, "camera", cam);
        if (used_obj != NULL) {
            float fscale = 1.0f;
            if (used_obj->type == TYPE_SHOTGUN) fscale = 0.75f;
            texture_t* t = texture_obj(obj_sprites, type_to_sprite(used_obj->type));
            draw_texture_transformed(texshader, t, col, SCREEN_XSCALE - 32, SCREEN_YSCALE - 16, t->width, t->height, 1.0f, 0.0f);
            if (used_obj->slot != NONE) {
                sprintf(&draw_string[0], "%u", used_obj->slot);
                draw_font_string(fontshader, &draw_string[0], font, SCREEN_XSCALE - 8, SCREEN_YSCALE - 16, 0.2, 0.0, color(1.0f, 0.2f, 0.2f, 1.0f));
            }
        }
        if (jetpack != NULL) {
            texture_t* t = texture_obj(obj_sprites, SPRITE_JETPACK);
            draw_texture_transformed(texshader, t, col, 8, SCREEN_YSCALE - 16, t->width, t->height, 1.0f, 0.0f);
            sprintf(&draw_string[0], "%u", jetpack->hp);
            draw_font_string(fontshader, &draw_string[0], font, 32, SCREEN_YSCALE - 16, 0.2, 0.0, color(1.0f, 0.2f, 0.2f, 1.0f));
        }
        sprintf(&draw_string[0], "%u", user->hp);
        draw_font_string(fontshader, &draw_string[0], font, SCREEN_XSCALE - 8, SCREEN_YSCALE - 8, 0.2, 0.0, color(0.2f, 0.2f, 1.0f, 1.0f));

        if (!client->connected) {
            draw_font_string(fontshader, "You   are   disconnected", font, SCREEN_XSCALE / 2, 16, 0.2, 0.0, color(0.4f, 0.4f, 1.0f, 1.0f));
            if (user->state == STATE_EVENT1_SEND) {
                object_shoot(bullets, obj_sprites, used_obj, smoke);
                user->state = STATE_LIVE;
            }
        } else if (!received) {
            received = NNetHost_read(client, 0);
            if (received) {
                unsigned int size = received / sizeof(packet_t);
                net_parse(client, user, peers, objects, my_objects, post_objects, pre_objects, static_objects, events, size, smoke, obj_sprites, bullets);
            }
            
            node_t* node = peers->head;
            while (node != NULL) {
                packet_t* peer = (packet_t*)node->data;
                if (peer != user) {
                    if (peer->index == KID_RUNNING && smoke_alarm < 0.0f) {
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), peer->x, peer->y - smoke_floor_h_offset);
                    }
                    if (peer->count == IM_SLIDING && smoke_alarm < 0.0f) {
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), peer->x + (peer->rot * smoke_floor_w_offset), peer->y - smoke_floor_h_offset);
                    }
                    if (peer->count == IM_DASHING && shadow_alarm < 0.0f) {
                        emit_shadow(shadows, sprite_by_id(color_players, peer->id, peer->index), peer->x, peer->y, peer->rot);
                    }
                    if (peer->count == IM_JETPACKING && smoke_alarm < 0.0f) {
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), peer->x, peer->y - smoke_floor_h_offset);
                    }
                }
                node = node->next;
            }

            if (events->size > 0) {
                node = events->tail;
                node_t* prev;
                while (node != NULL) {
                    prev = node->prev;
                    packet_t* ev = (packet_t*)node->data;
                    if (ev->state == STATE_EVENT1_RECEIVE) {
                        if (ev->index == EVENT_HIT || ev->index == EVENT_USER_HIT) {
                            emit_explotion(explotions, sprite_obj(obj_sprites, SPRITE_EXPLOTION), ev->x, ev->y, ev->rot);
                            if (ev->index == EVENT_USER_HIT) {
                                emit_blood(blood, sprite_obj(obj_sprites, SPRITE_BLOOD), 8, ev->x, ev->y);
                            }
                            //remove_entity_by_id(bullets, (unsigned int)ev->count);
                        } else if (ev->index == EVENT_EXPLOTION) {
                            emit_explotion(explotions, sprite_obj(obj_sprites, SPRITE_EXPLOTION), ev->x, ev->y, ev->rot);
                            remove_from_lists(objects, my_objects, static_objects, pre_objects, post_objects, throw_packets, ev->count);
                            /*if (granade != NULL) {
                                free(granade);
                                granade = NULL;
                            }*/
                        } else if (ev->index == EVENT_OPEN_BOX) {
                            emit_big_smoke(post_smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), ev->x, ev->y);
                            remove_from_lists(objects, my_objects, static_objects, pre_objects, post_objects, throw_packets, ev->count);
                        } else if (ev->index == EVENT_MEDKIT) {
                            unsigned int dif = max_hp - user->hp;
                            user->hp += ev->hp * (ev->hp < dif) + dif * (ev->hp >= dif);
                            remove_from_lists(objects, my_objects, static_objects, pre_objects, post_objects, throw_packets, ev->count);
                        } else if (ev->index == EVENT_AMMO) {
                            if (used_obj != NULL) object_add_ammo(used_obj);
                            remove_from_lists(objects, my_objects, static_objects, pre_objects, post_objects, throw_packets, ev->count);
                        }
                        list_remove_node(events, node);
                    }
                    node = prev;
                }
            }
        }

        dammage = (user->hp) * (user->hp < dammage) + (dammage) * (user->hp >= dammage);
        user->hp -= dammage;
        if (user->hp == 0) is_dead = true;

        // Game Input & Logic
        user->index = KID_STANDING;
        wall_sliding = false;
        is_grounded = place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(0.0f, -player->velocity.y * delta_time));

        //Side to side collision
        if (!is_dead && (keyboard_down(GLFW_KEY_D) || keyboard_down(GLFW_KEY_RIGHT))) {
            if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new((player->velocity.x + run_speed) * delta_time, floor_collision_offset))) {
                player->rect.x += run_speed * delta_time;
                user->rot = 1.0f;
                if (is_grounded && smoke_alarm < 0.0f) {
                    emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x, player->rect.y - smoke_floor_h_offset);
                }
                user->index = KID_RUNNING;
            }
            //Wallslide - Walljump
            else if (!is_grounded) {
                if (place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new((player->velocity.x + run_speed) * delta_time, -16.f))) {
                    if (player->velocity.y > wall_slide_speed) {
                        player->velocity.y = wall_slide_speed;
                        wall_sliding = true;
                        can_dash = true;
                        if (smoke_alarm < 0.0f) {
                            emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x + (user->rot * smoke_floor_w_offset), player->rect.y - smoke_floor_h_offset);
                        }
                    }
                }
                if (keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP)) {
                    if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(-wall_jump_dash * delta_time, -wall_jump_force * delta_time))) {
                        player->velocity.y = wall_jump_force;
                        player->velocity.x = -wall_jump_dash;
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x + (user->rot * smoke_floor_w_offset), player->rect.y - 6.0f);
                        can_double_jump = true;
                        can_dash = true;
                    }
                }
            }
            camera_alarm = 1.0f;
        }
        if (!is_dead && (keyboard_down(GLFW_KEY_A) || keyboard_down(GLFW_KEY_LEFT))) {
            if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new((player->velocity.x - run_speed) * delta_time, floor_collision_offset))) {
                player->rect.x -= run_speed * delta_time;
                user->rot = -1.0f;
                if (is_grounded && smoke_alarm < 0.0f) {
                    emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x, player->rect.y - smoke_floor_h_offset);
                }
                user->index = KID_RUNNING;
            }
            //Wallslide & walljump
            else if (!is_grounded) {
                if (place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new((player->velocity.x - run_speed) * delta_time, -16.f))) {
                    if (player->velocity.y > wall_slide_speed) {
                        player->velocity.y = wall_slide_speed;
                        wall_sliding = true;
                        if (smoke_alarm < 0.0f) {
                            emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x + (user->rot * smoke_floor_w_offset), player->rect.y - smoke_floor_h_offset);
                        }
                    }
                }
                if (keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP)) {
                    if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(wall_jump_dash * delta_time, -wall_jump_force * delta_time))) {
                        player->velocity.y = wall_jump_force;
                        player->velocity.x = wall_jump_dash;
                        emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x + (user->rot * smoke_floor_w_offset), player->rect.y - 6.0f);
                        can_double_jump = true;
                    }
                }
            } 
            camera_alarm = 1.0f;
        } 

        // Dash
        if (can_dash && !is_dead) {
            if (keyboard_pressed(GLFW_KEY_D)) {
                if (right_dash_timer > 0.0f) {
                    if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(dash_speed * delta_time, floor_collision_offset))) {
                        player->velocity.x = dash_speed;
                        can_dash = false;
                        is_dashing = can_dash_wait;
                    }
                } else right_dash_timer = double_click;
                camera_alarm = 1.0f;
            }
            else if (keyboard_pressed(GLFW_KEY_A)) {
                if (left_dash_timer > 0.0f) {
                    if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(-dash_speed * delta_time, floor_collision_offset))) {
                        player->velocity.x = -dash_speed;
                        can_dash = false;
                        is_dashing = can_dash_wait;
                    }
                } else left_dash_timer = double_click;
                camera_alarm = 1.0f;
            }
        }
        if (right_dash_timer > 0.0f) right_dash_timer -= delta_time;
        if (left_dash_timer > 0.0f) left_dash_timer -= delta_time;
        if (is_dashing > 0.0f) {
            if (shadow_alarm < 0.0f) {
                emit_shadow(shadows, sprite_by_id(color_players, user->id, user->index), player->rect.x, player->rect.y, user->rot);
            }
            player->velocity.y = 0.0f;
            user->index = KID_JUMPING;
            camera_alarm = 1.0f;
        }
        else if (is_grounded || wall_sliding) can_dash = true;
        is_dashing -= delta_time;

        //Horizontal Velocity
        if (player->velocity.x > -0.01f || player->velocity.y < 0.01f) {
            float lerp = lerpf(player->velocity.x, 0.0f, 0.1f);
            if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(lerp * delta_time, floor_collision_offset))) {
                player->velocity.x = lerp;
            } else {
                player->velocity.x = -lerp / 2.0f;
                is_dashing = 0;
            }
            //camera_alarm = 1.0f;
        } else player->velocity.x = 0.0f;
        player->rect.x += player->velocity.x * delta_time;

        //Sprite jump and fall index
        if (player->velocity.y > 0.01f) user->index = KID_FALLING;
        else if (player->velocity.y < -0.01f) user->index = KID_JUMPING;

        //Floor collision and gravity
        if (is_grounded && (player->velocity.y > -0.1f || player->velocity.y < 0.01f)) {
            player->velocity.y = 0.0f;
        } else if (is_dashing < can_dash_wait / 2) {
            player->velocity.y += gravity;
        }

        //Jump
        if (!is_dead && (keyboard_pressed(GLFW_KEY_W) || keyboard_pressed(GLFW_KEY_UP))) {
            if (is_grounded) {
                if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(0.0f, -jump_force * delta_time))) {
                    player->velocity.y = jump_force;
                    emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x, player->rect.y - smoke_floor_h_offset);
                    can_double_jump = true;
                    can_dash = true;
                    camera_alarm = 1.0f;
                }
            } else if (can_double_jump) {
                if (!place_meeting_entity_array_offset(blocks->data, blocks->used, player, vec2_new(0.0f, -double_jump_force * delta_time))) {
                    player->velocity.y = double_jump_force;
                    emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x, player->rect.y - smoke_floor_h_offset);
                    can_double_jump = false;
                    camera_alarm = 1.0f;
                }
            }
        }
        player->rect.y -= player->velocity.y * delta_time;
        if (smoke_alarm < 0.0f) smoke_alarm = 1.0f;
        if (shadow_alarm < 0.0f) shadow_alarm = 1.0f;
        
        if (!is_dead) {
            if (is_dashing > 0.0f) user->count = IM_DASHING;
            else if (wall_sliding) user->count = IM_SLIDING;
            else user->count = IM_NOTHING;
        }
        
        //Gun
        node_t* node = my_objects->head, *next;
        while (node != NULL) {
            next = node->next;
            packet_t* obj = *(packet_t**)node->data;
            if (obj != used_obj && obj != jetpack) {
                if (obj->state == STATE_CONNECT) {
                    obj->state = STATE_LIVE;
                }
                if (obj->index != user->id) {
                    list_remove_node(my_objects, node);
                    node = next;
                    continue;
                }

                if (obj->state == STATE_EVENT2_SEND) {
                    BUG;
                    update_throw_object(throw_objects, throw_packets, user->id, blocks, events, delta_time);
                } else if (obj->state == STATE_LIVE) {
                    texture_t* t = texture_obj(obj_sprites, type_to_sprite(obj->type));
                    entity_t e = entity_new(0, obj->x, obj->y, t->width, t->height);
                    bool collision = place_meeting_entity_array_offset(blocks->data, blocks->used, &e, vec2_new(0.0f, -obj->rot * delta_time));
                    if (collision && (obj->rot > -0.1f || obj->rot < 0.01f)) {
                        obj->rot = 0.0f;
                        //obj->index = NONE;
                        if (obj->type != TYPE_BOX) obj->index = NONE; //FIX!!!
                    }
                    else obj->rot += gravity;
                    obj->y -= obj->rot * delta_time;
                }
            }
            node = next;
        }
        
        //update_used_object(&used_obj, user, throw_objects, throw_packets, player, obj_sprites, mouse);
        
        if (!is_dead && keyboard_pressed(GLFW_KEY_SPACE)) {
            bool picked = false;
            node = objects->head;
            while (node != NULL) {
                packet_t* obj = (packet_t*)node->data;
                if (obj->type == TYPE_AMMO && obj->state == STATE_LIVE) {
                    texture_t* t = texture_obj(obj_sprites, SPRITE_AMMO);
                    entity_t e = entity_new(t->id, obj->x, obj->y, t->width, t->height);
                    if (place_meeting(player->rect, e.rect)) {
                        if (used_obj != NULL && used_obj->slot != NONE) {
                            if (packet_find_by_id_ptr(my_objects, obj->id) == NULL) list_push(my_objects, &obj);
                            obj->index = user->id;
                            obj->state = STATE_CONNECT;
                            packet_t ev = packet_event(user->id, EVENT_AMMO, obj->id, obj->hp, obj->x, obj->y, obj->rot);
                            list_push(events, &ev);
                            picked = true;
                        }
                    }
                }
                if (obj->type == TYPE_MEDKIT && obj->state == STATE_LIVE) {
                    texture_t* t = texture_obj(obj_sprites, SPRITE_MEDKIT);
                    entity_t e = entity_new(t->id, obj->x, obj->y, t->width, t->height);
                    if (place_meeting(player->rect, e.rect)) {
                        if (user->hp < max_hp) {
                            if (packet_find_by_id_ptr(my_objects, obj->id) == NULL) list_push(my_objects, &obj);
                            obj->index = user->id;
                            obj->state = STATE_CONNECT;
                            packet_t ev = packet_event(user->id, EVENT_MEDKIT, obj->id, obj->hp, obj->x, obj->y, obj->rot);
                            list_push(events, &ev);
                            picked = true;
                        }
                    }
                }
                else if (obj->type == TYPE_JETPACK && obj->state == STATE_LIVE) {
                    texture_t* t = texture_obj(obj_sprites, SPRITE_JETPACK);
                    entity_t e = entity_new(t->id, obj->x, obj->y, t->width, t->height);
                    if (place_meeting(player->rect, e.rect)) {
                        if (jetpack != NULL) {
                            jetpack->state = STATE_LIVE;
                            jetpack->count = 2;
                            jetpack->rot = 0.0f;
                            jetpack = NULL;
                        }
                        obj->state = STATE_CONNECT;
                        if (obj->index != user->id) {
                            obj->index = user->id;
                            if (packet_find_by_id_ptr(my_objects, obj->id) == NULL) list_push(my_objects, &obj);
                        }
                        jetpack = obj;
                        picked = true;
                        break;
                    }
                }
                else if (obj->state == STATE_LIVE && obj->type != TYPE_FIRE_BARREL) {
                    texture_t* t = texture_obj(obj_sprites, SPRITE_GUN);
                    entity_t e = entity_new(t->id, obj->x, obj->y, t->width, t->height);
                    if (place_meeting(player->rect, e.rect)) {
                        if (used_obj != NULL) {
                            drop_object(&used_obj, user);
                        }
                        pick_object(&used_obj, obj, my_objects, user);
                        picked = true;
                        break;
                    }
                }
                node = node->next;
            }
            if (!picked) {
                if (used_obj != NULL) {
                    drop_object(&used_obj, user);
                } else if (jetpack != NULL) {
                    jetpack->state = STATE_LIVE;
                    jetpack->rot = 0.0f;
                    jetpack->count = 2;
                    jetpack = NULL;
                }
            }
        }

        //jetpack
        if (jetpack != NULL) {
            jetpack->state = STATE_CONNECT;
            jetpack->x = player->rect.x - user->rot * 6.0f;
            jetpack->y = player->rect.y;
            jetpack->count = (uint8_t)((int)user->rot + 1);
            if (keyboard_down(GLFW_KEY_LEFT_SHIFT)) {
                if (jetpack->hp != 0) {
                    player->velocity.y -= jet_pack_force;
                    jetpack->hp --;
                    if (smoke_alarm == 1.0f) emit_smoke(smoke, sprite_obj(obj_sprites, SPRITE_SMOKE), player->rect.x, player->rect.y - 6.0f);
                    user->count = IM_JETPACKING;
                } else {
                    jetpack->state = STATE_LIVE;
                    jetpack->rot = 0.0f;
                    jetpack->count = 2;
                    jetpack = NULL;
                }
                camera_alarm = 1.0f;
            }
        }

        //DIE
        if (is_dead) {
            draw_font_string(fontshader, "You  died", font, SCREEN_XSCALE / 2, SCREEN_YSCALE / 2 - 48, 0.4, 0.0, color(1.0f, 0.2f, 0.2f, 1.0f));
            if (used_obj != NULL) drop_object(&used_obj, user);
            if (jetpack != NULL) {
                jetpack->state = STATE_LIVE;
                jetpack->count = 2;
                jetpack->rot = 0.0f;
                jetpack = NULL;
            }
            user->index = KID_DEAD;
            user->count = IM_DEAD;
            user->state = STATE_LIVE;
            player->rot = deg_to_rad(90.0f);
            player->rect.w = (float)player_texture->width * 0.5f;
            player->rect.h = (float)player_texture->height * 0.5f;
            camera_alarm = -1.0f;
            dead_timer -= delta_time;
            if (dead_timer < 0.0f) {
                player_respawn(player, user, player_texture);
                dead_timer = dead_wait;
                is_dead = false;
            }
        } 

        if (received != 0) {
            user->x = player->rect.x;
            user->y = player->rect.y;

            packet_t* p = (packet_t*)client->buffer;
            memcpy(p, user, sizeof(packet_t));
            p++;

            node = my_objects->head;
            for (packet_t* end = p + my_objects->size; p != end; p++) {
                packet_t* obj = *(packet_t**)node->data;
                memcpy(p, obj, sizeof(packet_t));
                node = node->next;
            }

            unsigned int events_size = 0;
            node = events->head;
            while(node != NULL) {
                node_t* next = node->next;
                packet_t* ev = (packet_t*)node->data;
                if (ev->state == STATE_EVENT1_SEND) {
                    memcpy(p, ev, sizeof(packet_t));
                    list_remove_node(events, node);
                    p++;
                    events_size++;
                }
                node = next;
            }

            unsigned int size = (my_objects->size + events_size + 1) * sizeof(packet_t);
            NNet_send(client->server, client->packet, client->buffer, size, 0);
            if (events->size > 0) list_free(events);
            received = 0;
        }
        screen_refresh();
    }
    if (client->connected) NNet_disconnect(client->host, client->server, &client->event, TIMEOUT);
    exitNanoNet(client->host);
    NNetHost_free(client);

    list_free(peers);
    list_free(objects);
    list_free(events);
    list_free(my_objects);
    list_free(pre_objects);
    list_free(post_objects);
    list_free(static_objects);
    list_free(throw_objects);
    list_free(post_smoke);
    list_free(throw_packets);

    free(peers);
    free(objects);
    free(events);
    free(my_objects);
    free(pre_objects);
    free(post_objects);
    free(static_objects);
    free(post_smoke);
    free(throw_objects);
    free(throw_packets);

    free(user);
    free(player);
    free(font);

    array_free(colors);
    free_array_of_sprites(obj_sprites);
    array_free(parallax_sprites);
    free_array_of_array(color_players);
    free(colors);
    free(color_players);
    free(obj_sprites);
    free(parallax_sprites);

    array_free(blocks);
    array_free(clouds);

    list_free(bullets);
    list_free(smoke);
    list_free(shadows);
    list_free(explotions);
    list_free(blood);
    free(bullets);
    free(blocks);
    free(smoke);
    free(clouds);
    free(shadows);
    free(explotions);
    free(blood);

    exitCore();

    zbug_diagnostic();
    zbug_exit();
    printf("Goodbye and thank you!\n");
    return EXIT_SUCCESS;
}
