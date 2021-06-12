#include <Heart.h>
#define BUG printf("%s - line %u - func: %s\n", __FILE__, __LINE__, __func__)

void list_reduce(list_t* list)
{
    if (!list->head) return;

    node_t* node = list->head;
    while (node != NULL) {
        node_t* nnode = list->head, *next;
        while (nnode != NULL) {
            next = nnode->next;
            if (node != nnode) {
                if (nnode->data == NULL) BUG;
                printf("%p, %p, %p\n", NULL, node->data, nnode->data);
                BUG;
                vec2 v = *(vec2*)node->data;
                BUG;
                vec2 vv = *(vec2*)nnode->data;
                BUG;
                printf("%f, %f - %f, %f\n", v.x, v.y, vv.x, vv.y);
                BUG;
                if (vec2_cmp(v, vv)) {
                    BUG;
                    list_remove_node(list, nnode);
                    BUG;
                    printf("Element was removed from list\n");
                    BUG;
                }
                BUG;
            }
            BUG;
            nnode = next;
            BUG;
        }
        BUG;
        node = node->next;
        BUG;
    }
    printf("List reduced succesfully\n");
    printf("Size of list: %u\n", list->size);
}

void write_map_file(const char* path, list_t* list)
{
    FILE* file = fopen(path, "w");
    if (!file) {
        printf("Could not write file '%s'\n", path);
        return;
    }

    node_t* node = list->head;
    while (node != NULL) {
        vec2 v = *(vec2*)node->data;
        fprintf(file, "%f, %f\n", v.x, v.y);
        node = node->next;
    }
    fclose(file);
    printf("Succesfully writed file '%s'\n", path);
}

list_t* read_map_file(const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        printf("Could not read file '%s'\n", path);
        return NULL; 
    }

    list_t* list = list_new(sizeof(vec2));
    while (1) {
        vec2 v;
        int res = fscanf(file, "%f, %f\n", &v.x, &v.y);
        if (res == EOF) break;
        list_add(list, &v);
    }
    fclose(file);
    return list;
}

int main()
{
    initCore();
    window_create("2D Platformer", 800, 600, 400, 100, false);
    core.scale = 4;
    
    texture_t tile = texture_load("assets/sprites/tile.png");

    vertex_array_buffer_quad_create();
    unsigned int scale = core.scale * 4;
    unsigned int shader = shader_load("assets/shaders/texvert.frag", "assets/shaders/texfrag.frag");
    
    vec2 cursor = {0.0f, 0.0f};
    vec4 trans = {0.0f, 0.0f, (float)tile.width, (float)tile.height};
    vec3 res = {(float)core.width, (float)core.height, (float)core.scale};
    vec4 cam = {0.0f, 0.0f, 1.0f, 0.0f};
    color_t white = {1.0f, 1.0f, 1.0f, 1.0f};
    color_t background = {0.3f, 0.3f, 1.0f, 1.0f};

    shader_set_uniform_float(shader, 3, "resolution", &res.x);
    shader_set_uniform_float(shader, 4, "camera", &cam.x);
    shader_set_uniform_float(shader, 2, "scale_rot", &cam.z);
    shader_set_uniform_float(shader, 4, "color", &white.r);
    glBindTexture(GL_TEXTURE_2D, tile.id);

    list_t* map_blocks = read_map_file("assets/maps/new.txt");
    node_t* node = NULL;

    const float speed = 50.0f;
    float last_time = time_start_timer(), delta_time;
    while (window_is_open()) {
        delta_time = time_get_delta_time(&last_time);
        
        screen_clear(&background.r);
        mouse_get_position(&cursor.x, &cursor.y);
        cursor = vec2_mult(to_vec2(to_ivec2(vec2_div(vec2_add(cursor, *(vec2*)&cam), core.scale * 4))), core.scale * 4);

        if (keyboard_down(GLFW_KEY_D)) {
            cam.x += delta_time * speed;
            shader_set_uniform_float(shader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_A)) {
            cam.x -= delta_time * speed;
            shader_set_uniform_float(shader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_W)) {
            cam.y += delta_time * speed;
            shader_set_uniform_float(shader, 4, "camera", &cam.x);
        }
        if (keyboard_down(GLFW_KEY_S)) {
            cam.y -= delta_time * speed;
            shader_set_uniform_float(shader, 4, "camera", &cam.x);
        }
        if (keyboard_pressed(GLFW_KEY_P)) write_map_file("assets/maps/new.txt", map_blocks);
        if (keyboard_pressed(GLFW_KEY_O)) list_reduce(map_blocks);
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_pressed(GLFW_KEY_R)) {
            list_free(map_blocks);
            cam.x = 0.0f;
            cam.y = 0.0f;
        }
        if (mouse_pressed(GLFW_MOUSE_BUTTON_LEFT)) {
            list_add(map_blocks, &cursor);
            printf("Size of list: %u\n", map_blocks->size);
        }

        memcpy(&trans, &cursor, sizeof(vec2));
        shader_set_uniform_float(shader, 4, "trans", &trans.x);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        node = map_blocks->head;
        while (node != NULL) {
            memcpy(&trans, node->data, sizeof(vec2));
            shader_set_uniform_float(shader, 4, "trans", &trans.x);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            node = node->next;
        }

        screen_refresh();
    }
    list_free(map_blocks);
    free(map_blocks);
    exitCore();
    return 0;
}