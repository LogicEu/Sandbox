#include <math.h>
#include <Heart.h>

#define BUG printf("File: %s, Line: %u, Func: %s\n", __FILE__, __LINE__, __func__)

unsigned int global_seed, lightshader, texshader, framebuffershader, skyboxshader, fontshader;
const float half_pi = 3.14f / 2.0f;

void vertex_element_buffer_obj(unsigned int id, array_t* vertices, array_t* indices, obj_flag type)
{
    if (!vertices || !indices) return;
    unsigned int VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(id);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->used * indices->bytes, indices->data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)0);
    if (type == OBJ_VTN) {
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, uv));
    }
    if (type != OBJ_V) {
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, normal));
    }
}

void vmesh_bind(unsigned int id, vmesh_t* mesh)
{
    if (!mesh) return;
    vertex_element_buffer_obj(id, mesh->vertices, mesh->indices, mesh->type);
}

void perlin_mesh(mesh_t* mesh, float mult)
{   
    for (int i = 0; i < mesh->vertices->used; i++) {
        vec3* v = (vec3*)array_index(mesh->vertices, i);
        v->y = perlin2d(v->x, v->z, 0.01, 4, global_seed) * mult;
    }
    mesh->normals = vec3_face_normal_array(mesh->vertices);
}

void perlin_vmesh(vmesh_t* mesh, float mult)
{
    for (int i = 0; i < mesh->vertices->used; i++) {
        vertex_t* v = (vertex_t*)array_index(mesh->vertices, i);
        vec3 pos = v->position;
        v->position.y = perlin2d(pos.x, pos.z, 0.01, 4, global_seed) * mult;
    }
    mesh->type = OBJ_VN;
    vertex_array_set_face_normal(mesh->vertices, mesh->indices);
}

uint8_t lerpu8(uint8_t a, uint8_t b, float f)
{
    return (uint8_t)lerpf((float)a, (float)b, f);
}

pixel_t pixel_lerp(pixel_t p1, pixel_t p2)
{
    pixel_t p;
    p.r = lerpu8(p1.r, p2.r, 0.5f);
    p.g = lerpu8(p1.g, p2.g, 0.5f);
    p.b = lerpu8(p1.b, p2.b, 0.5f);
    p.a = lerpu8(p1.a, p2.a, 0.5f);
    return p;
}

void scale_gradient_bitmap(bitmap_t** bitmap)
{
    bitmap_t* b = bitmap_new((*bitmap)->width * 2 - 1, 1);
    for (int i = 0; i < b->width; i++) {
        if (i % 2 == 0) {
            *pixel_at(b, i, 0) = *pixel_at(*bitmap, i / 2, 0);
        } else {
            int min = (i - 1) / 2;
            int max = (i + 1) / 2;
            min *= (min > 0);
            if (max > (*bitmap)->width) max = (*bitmap)->width;
            *pixel_at(b, i, 0) = pixel_lerp(*pixel_at(*bitmap, min, 0), *pixel_at(*bitmap, max, 0));
        }
    }
    bitmap_free(*bitmap);
    *bitmap = b;
}

void scale_gradient_bitmap_irregular(bitmap_t** bitmap)
{
    bitmap_t* b = bitmap_new((*bitmap)->width * 2 - 1, 1);
    for (int i = 0; i < b->width; i++) {
        if (i % 2 == 0) {
            *pixel_at(b, i, 0) = *pixel_at(*bitmap, i / 2, 0);
        } else {
            int min = i / 2 - 1;
            int max = i / 2 + 1;
            min *= (min > 0);
            if (max > (*bitmap)->width) max = (*bitmap)->width;
            *pixel_at(b, i, 0) = pixel_lerp(*pixel_at(*bitmap, min, 0), *pixel_at(*bitmap, max, 0));
        }
    }
    bitmap_free(*bitmap);
    *bitmap = b;
}

bitmap_t* gradient_planet()
{
    bitmap_t* bitmap = bitmap_new(6, 1);
    *pixel_at(bitmap, 0, 0) = pixel(255, 255, 255, 255); //White
    *pixel_at(bitmap, 1, 0) = pixel(250, 248, 220, 255); //Clear Brown
    *pixel_at(bitmap, 2, 0) = pixel(185, 145, 125, 255); //Brown
    *pixel_at(bitmap, 3, 0) = pixel(85, 45, 25, 255); //Brown
    *pixel_at(bitmap, 4, 0) = pixel(105, 85, 65, 255); //Grey
    *pixel_at(bitmap, 5, 0) = pixel(255, 255, 255, 255); //White
    return bitmap;
}

bitmap_t* gradient_bitmap()
{
    bitmap_t* bitmap = bitmap_new(8, 1);
    *pixel_at(bitmap, 0, 0) = pixel(100, 100, 255, 255); //Blue
    *pixel_at(bitmap, 1, 0) = pixel(250, 248, 220, 255); //Clear Brown
    *pixel_at(bitmap, 2, 0) = pixel(105, 255, 105, 255); //Lighter Green
    *pixel_at(bitmap, 3, 0) = pixel(55, 255, 55, 255); //Green
    *pixel_at(bitmap, 4, 0) = pixel(0, 225, 0, 255); //Dark Green
    *pixel_at(bitmap, 5, 0) = pixel(185, 145, 125, 255); //Brown
    *pixel_at(bitmap, 6, 0) = pixel(105, 85, 65, 255); //Grey
    *pixel_at(bitmap, 7, 0) = pixel(255, 255, 255, 255); //White
    return bitmap;
}

void bitmap_smooth(bitmap_t* bitmap, const unsigned int smooth)
{
    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            if (pixel_at(bitmap, x, y)->r > 105 && rand() % smooth != 0) {
                if (x + 1 < bitmap->width) *pixel_at(bitmap, x + 1, y) = pixel_lerp(*pixel_at(bitmap, x + 1, y), *pixel_at(bitmap, x, y));
                if (x - 1 >= 0) *pixel_at(bitmap, x - 1, y) = pixel_lerp(*pixel_at(bitmap, x - 1, y), *pixel_at(bitmap, x, y));
                if (y + 1 < bitmap->height) *pixel_at(bitmap, x, y + 1) = pixel_lerp(*pixel_at(bitmap, x, y + 1), *pixel_at(bitmap, x, y));
                if (y - 1 >= 0) *pixel_at(bitmap, x, y - 1) = pixel_lerp(*pixel_at(bitmap, x, y - 1), *pixel_at(bitmap, x, y));
            }
        }
    }
}

bitmap_t* space_bitmap(const unsigned int w, const unsigned int h, const unsigned int n, const unsigned in, const unsigned int smooth)
{
    bitmap_t* bitmap = bitmap_color(w, h, pixel(0, 0, 0, 0));
    for (int y = 0; y < bitmap->height; y++) {
        for (int x = 0; x < bitmap->width; x++) {
            if (rand() % in > n) *pixel_at(bitmap, x, y) = pixel(255, 255, 255, 255);
        }
    }
    for (int i = 0; i < smooth; i++) {
        bitmap_smooth(bitmap, smooth);
    }
    return bitmap;
}

texture_t space_cubemap(const unsigned int w, const unsigned int h, const unsigned int n, const unsigned int in, const unsigned int smooth)
{
    texture_t texture;
    texture.width = w;
    texture.height = h;
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture.id);

    for (int i = 0; i < 6; i++) {
        bitmap_t* bitmap = space_bitmap(w, h, n, in, smooth);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap->pixels);
        bitmap_free(bitmap);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
}

skybox_t* space_skybox(const unsigned int w, const unsigned int h, const unsigned int n, const unsigned int in, const unsigned int smooth)
{
    skybox_t* skybox = (skybox_t*)malloc(sizeof(skybox_t));
    skybox->cubemap = space_cubemap(w, h, n, in, smooth);
    skybox->VAO = vertex_array_buffer_skybox_create();
    return skybox;
}

void shaders_init_this()
{
    unsigned int w, h, s;
    core_get_size(&w, &h);
    s = core_get_2d_scale();
    float width = (float)(int)w, height = (float)(int)h, scale = (float)(int)s;

    texshader = shader_load("assets/shaders/texvert.frag", "assets/shaders/texfrag.frag");
    glUniform3f(glGetUniformLocation(texshader, "resolution"), width, height, scale);
    glUniform4f(glGetUniformLocation(texshader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);

    fontshader = shader_load("assets/shaders/fontvert.frag", "assets/shaders/fontfrag.frag");
    glUniform3f(glGetUniformLocation(fontshader, "resolution"), width, height, scale);
    glUniform4f(glGetUniformLocation(fontshader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);

    lightshader = shader_load("assets/shaders/lightv.frag", "assets/shaders/light.frag");
    glUseProgram(lightshader);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.direction"), -0.5f, -1.0f, -0.5f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.ambient"), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightshader, "global_light.specular"), 0.7f, 0.7f, 0.7f);

    glUniform3f(glGetUniformLocation(lightshader, "point_light.position"), 4.0f, 8.0f, 4.0f); 
    glUniform3f(glGetUniformLocation(lightshader, "point_light.ambient"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightshader, "point_light.diffuse"), 1.0f, 1.0f, 1.0f);
    glUniform3f(glGetUniformLocation(lightshader, "point_light.specular"), 1.0f, 1.0f, 1.0f);

    glUniform1f(glGetUniformLocation(lightshader, "point_light.constant"), .01f);
    glUniform1f(glGetUniformLocation(lightshader, "point_light.linear"), 0.01f);
    glUniform1f(glGetUniformLocation(lightshader, "point_light.quadratic"), 0.01f);
    glUniform1f(glGetUniformLocation(lightshader, "shininess"), 32.0f);

    //framebuffershader = shader_load("shaders/framebufferv.frag", "shaders/framebufferf.frag");
    skyboxshader = shader_load("assets/shaders/skyboxvert.frag", "assets/shaders/skyboxfrag.frag");
}

int main()
{
    coreInit();
    window_create("Space", 800, 600, 0, 0);
    core_set_3d_mode();
    shaders_init_this();

    unsigned int quad = vertex_array_buffer_quad_create();
    global_seed = rand_uint(rand_uint(time(NULL)));
    printf("Seed: %u\n", global_seed);

    unsigned int draw_kind = 4;
    float scale = 1.0f, rot = 0.0f;
    vec3 position = {0., 3., -5.};

    float v = 0.0f, h = 0.0f;
    const float mouse_speed = 0.005f;
    const float speed = 15.0f;

    color_t background = {0.0f, 0.0f, 0.2f, 1.0f};
    texture_t tile = texture_load("assets/sprites/tile.png");
    font_t* font = font_load("assets/fonts/arcadeclassic.ttf", 24);

    float div = 32.0f;
    float last_time = time_start_timer();
    bitmap_t* bitmap = gradient_planet();
    for (int i = 0; i < 8; i++) {
        scale_gradient_bitmap_irregular(&bitmap);
    }
    texture_t planet = texture_from_bitmap(bitmap);
    vmesh_t* mesh2 = vmesh_shape_sphere(8);
    vmesh_t* mesh = vmesh_shape_sphere(4);
    vmesh_scale(mesh, 12.0f);
    vmesh_scale(mesh2, 6.0f);
    vmesh_move(mesh2, vec3_new(34.0f, 0.0f, 0.0f));
    vmesh_combine(mesh, mesh2);
    vmesh_height_color_gradient(mesh);
    unsigned int id_planet = vertex_array_buffer_id_new();
    mesh->type = OBJ_VTN;
    vmesh_bind(id_planet, mesh);
    printf("Planet mesh: %d, %d, %d\n", mesh->vertices->used, mesh->indices->used, id_planet);
    printf("Time: %f\n", time_end_timer(last_time));

    last_time = time_start_timer();
    bitmap = gradient_bitmap();
    for (int i = 0; i < 4; i++) {
        scale_gradient_bitmap(&bitmap);
    }
    texture_t gradient = texture_from_bitmap(bitmap);
    vmesh_t* plane = vmesh_shape_plane(100, 100);
    perlin_vmesh(plane, div);
    vmesh_smooth_optim(&plane);
    vmesh_height_color_gradient(plane);
    unsigned int id_floor = vertex_array_buffer_id_new();
    mesh->type = OBJ_VTN;
    vmesh_bind(id_floor, plane);
    printf("Plane mesh: %d, %d, %d\n", plane->vertices->used, plane->indices->used, id_floor);
    printf("Time: %f\n", time_end_timer(last_time));

    bool flag = true;
    skybox_t* skybox = space_skybox(1080, 1080, 998, 1000, 2);
    mat4 mat_id = mat4_id();

    //framebuffer_t* framebuffer = framebuffer_new();
    unsigned int _w, _h;
    core_get_size(&_w, &_h);
    float width = (float)(int)_w, height = (float)(int)_h;
    float aspect = width / height;
    float planet_rot = 0.0f;
    char delta_string[20] = "0.0000";
    int alarm = 20;

    screen_color(background.r, background.g, background.b, background.a);
    while(window_is_open()) {
        screen_clear();
        //set_3d_mode();
        //framebuffer_bind(framebuffer);

        float delta_time = time_get_delta_time(&last_time);
        float delta_speed = delta_time * speed;
        alarm --;
        if (alarm < 0) {
            sprintf(delta_string, "%f", delta_time);
            alarm = 30;
        }
        printf("%f\r", delta_time);

        mouse_get_position3d(&h, &v);
        h *= 0.01f;
        v *= 0.01f;

        vec3 direction = {cos(v) * sin(h), sin(v), cos(v) * cos(h)};
        vec3 right = {sin(h - half_pi), 0.0f, cos(h - half_pi)};
        vec3 up = vec3_cross(right, direction);

        mat4 projection = mat4_perspective_RH(deg_to_rad(45.0f), aspect, 0.1f, 100.0f);
        mat4 view = mat4_look_at_RH(position, vec3_add(position, direction), up);

        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_down(GLFW_KEY_D)) 
            position = vec3_add(position, vec3_mult(right, delta_speed));
        if (keyboard_down(GLFW_KEY_A)) 
            position = vec3_sub(position, vec3_mult(right, delta_speed));
        if (keyboard_down(GLFW_KEY_W)) 
            position = vec3_add(position, vec3_mult(direction, delta_speed));
        if (keyboard_down(GLFW_KEY_S)) 
            position = vec3_sub(position, vec3_mult(direction, delta_speed));
        if (keyboard_down(GLFW_KEY_Z)) 
            position = vec3_add(position, vec3_mult(up, delta_speed));
        if (keyboard_down(GLFW_KEY_X)) 
            position = vec3_sub(position, vec3_mult(up, delta_speed));
        if (keyboard_down(GLFW_KEY_UP)) position.z += delta_speed;
        if (keyboard_down(GLFW_KEY_DOWN)) position.z -= delta_speed;
        if (keyboard_down(GLFW_KEY_RIGHT)) position.x += delta_speed;
        if (keyboard_down(GLFW_KEY_LEFT)) position.x -= delta_speed;
        if (keyboard_down(GLFW_KEY_P)) scale += delta_time;
        if (keyboard_down(GLFW_KEY_O)) scale -= delta_time;
        if (keyboard_down(GLFW_KEY_K)) rot += delta_time;
        if (keyboard_down(GLFW_KEY_L)) rot -= delta_time;
        if (keyboard_pressed(GLFW_KEY_M)) {
            div++;
            perlin_vmesh(plane, div);
        }
        if (keyboard_pressed(GLFW_KEY_N)) {
            div--;
            perlin_vmesh(plane, div);
        }
        if (keyboard_pressed(GLFW_KEY_SPACE)) {
            if (flag) {
                background = color(0.3f, 0.3f, 1.0f, 1.0f);
                flag = false;
            }
            else {
                background = color(0.0f, 0.0f, 0.1f, 1.0f);
                flag = true;
            }
        }
        if (keyboard_pressed(GLFW_KEY_V)) {
            if (draw_kind == 0x0004) draw_kind = 0x0001;
            else draw_kind = 0x0004;
        }
        if (keyboard_pressed(GLFW_KEY_R)) {
            global_seed = rand_uint(rand());
            perlin_vmesh(plane, div);
        }
        if (keyboard_pressed(GLFW_KEY_P)) {
            vmesh_write_file(mesh, "assets/models/vmesh.obj");
        }
        if (keyboard_pressed(GLFW_KEY_O)) {
            vmesh_write_file_quick(mesh, "assets/models/vmeshq.obj");
        }
        if (keyboard_pressed(GLFW_KEY_B)) {
            vmesh_smooth_sphere(&mesh);
        }

        if (flag) {
            mat4 mat = mat4_mult(projection, mat4_mult(mat4_look_at(univec3(0.0f), direction, up), mat4_id()));
            glDepthMask(GL_FALSE);
            glUseProgram(skyboxshader);
            glBindVertexArray(skybox->VAO);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap.id);
            glUniformMatrix4fv(glGetUniformLocation(skyboxshader, "MVP"), 1, GL_FALSE, &mat.data[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glDepthMask(GL_TRUE);
        }

        planet_rot += delta_time;
        mat4 planet_model = mat4_model(vec3_new(20.0f, 0.0f, 20.0f), univec3(1.0f), vec3_new(0.0f, 1.0f, 0.0f ), planet_rot);
        
        glUseProgram(lightshader);
        glBindVertexArray(id_planet);
        glBindTexture(GL_TEXTURE_2D, planet.id);
        glUniform3f(glGetUniformLocation(lightshader, "point_light.position"), position.x, position.y, position.z);
        glUniform3f(glGetUniformLocation(lightshader, "viewPos"), position.x, position.y, position.z);
        glUniformMatrix4fv(glGetUniformLocation(lightshader, "view"), 1, GL_FALSE, &view.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightshader, "projection"), 1, GL_FALSE, &projection.data[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(lightshader, "model"), 1, GL_FALSE, &planet_model.data[0][0]);
        glDrawElements(draw_kind, mesh->indices->used, GL_UNSIGNED_INT, 0);

        glBindVertexArray(id_floor);
        glBindTexture(GL_TEXTURE_2D, gradient.id);
        glUniformMatrix4fv(glGetUniformLocation(lightshader, "model"), 1, GL_FALSE, &mat_id.data[0][0]);
        glDrawElements(draw_kind, plane->indices->used, GL_UNSIGNED_INT, 0);

        //glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //screen_clear(&background.r);
        //set_2d_mode();
        //glBindVertexArray(quad);
        //draw_framebuffer(&framebuffer->texture);
        //draw_texture(&planet, core.width / core.scale - 10, core.height / core.scale - 10, 1.0f, 0.0f);
        //draw_string("Gear Engine", font, 10, core.height / core.scale - 48, 1.0f, 0.0f, 
        //            color(0.5f, 0.5f, 1.0f, 1.0f));
        //draw_string(delta_string, font, 10, core.height / core.scale - 128, 1.0f, 0.0f, color(0.5f, 0.5f, 1.0f, 1.0f));

        screen_refresh();
    } 
    free(font);
    free(skybox);
    vmesh_free(mesh);
    vmesh_free(plane);
    coreExit();
    return 0;
}
