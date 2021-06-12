#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SCREEN_SCALE 4
#define SCREEN_XSCALE SCREEN_WIDTH / SCREEN_SCALE
#define SCREEN_YSCALE SCREEN_HEIGHT / SCREEN_SCALE
#define FULLSCREEN 0
#include <Fract/Fract.h>
#include <Core/Core.h>
#include <Imagine/Imagine.h>

static unsigned int shader;
static int moux, mouy, mx, my;
static pixel_t cursor;

void set_bitmap_to_alpha(bitmap_t* bitmap)
{
    pixel_t white = unipixel(255);
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            if (!memcmp(pixel_at(bitmap, x, y), &white, sizeof(pixel_t))) {
                *pixel_at(bitmap, x, y) = unipixel(0);
            }
        }
    }
}

void flood_fill(bitmap_t* bitmap, uint32_t x, uint32_t y, pixel_t* old_col, pixel_t* new_col)
{
    if (!memcmp(pixel_at(bitmap, x, y), old_col, sizeof(pixel_t))) {
        *pixel_at(bitmap, x, y) = *new_col;
        if (x + 1 < bitmap->width)flood_fill(bitmap, x + 1, y, old_col, new_col);
        if (x - 1 >= 0) flood_fill(bitmap, x - 1, y, old_col, new_col);
        if (y + 1 < bitmap->height) flood_fill(bitmap, x, y + 1, old_col, new_col);
        if (y - 1 >= 0) flood_fill(bitmap, x, y - 1, old_col, new_col);
    }
}

void draw_pixel(pixel_t p, float x, float y)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, x, y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), 1.0f, 0.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void draw_proto_pixel(pixel_t p, float x, float y, float origin_x, float origin_y, float scale, float rot)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, origin_x, origin_y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), scale, rot, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_transf_bitmap(bitmap_t* bitmap, int bitmap_x, int bitmap_y)
{
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            int xpos = bitmap_x + x - bitmap->width / 2;
            int ypos = bitmap_y + (bitmap->height - y) - bitmap->height/2;
            draw_pixel(*pixel_at(bitmap, x, y), xpos, ypos);
            if (xpos == moux && ypos == mouy) {
                if (glfwGetMouseButton(window_get(), GLFW_MOUSE_BUTTON_LEFT)) {
                    cursor = *pixel_at(bitmap, x, y);
                }
            }
        }
    }
}

void draw_trans_bitmap(bitmap_t* bitmap, int bitmap_x, int bitmap_y, float scale, float rot)
{
    for (unsigned int y = 0; y < bitmap->height; y++) {
        for (unsigned int x = 0; x < bitmap->width; x++) {
            int xpos = bitmap_x + x - bitmap->width / 2;
            int ypos = bitmap_y + (bitmap->height - y) - bitmap->height/2;
            draw_proto_pixel(*pixel_at(bitmap, x, y), xpos, ypos, bitmap_x, bitmap_y, scale, rot);
            draw_proto_pixel(cursor, mx, my, bitmap_x, bitmap_y, scale, rot);
            if (xpos == mx && ypos == my) {
                if (glfwGetMouseButton(window_get(), GLFW_MOUSE_BUTTON_LEFT)) {
                    if (keyboard_down(GLFW_KEY_SPACE)) cursor = *pixel_at(bitmap, x, y);
                    else *pixel_at(bitmap, x, y) = cursor;
                }
                if (keyboard_pressed(GLFW_KEY_F)) flood_fill(bitmap, x, y, pixel_at(bitmap, x, y), &cursor);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    char filename[200] = "sprite.png", sourcefile[200] = "$";
    uint32_t width = 32, height = 32;
    if (argc == 2) {
        if (!strcmp(argv[1], "-help")) {
            printf("Argument 1: width\nArgument 2: height\nArgument 3: file name\n");
            printf("Press 'R' to reinitialize the image.\n");
            printf("Press 'S' to re-scale the image.\n");
            printf("Press 'A' to set white color to alpha.\n");
            printf("Press 'P' to print the image to PNG format.\n");
            printf("Press 'Z', 'X' or 'C' and the arrow keys to change color.\n");
            exit(0);
        } else {
            strcpy(sourcefile, argv[1]);           
        }
    } else if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    } else if (argc > 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);
    }

    coreInit();
    core_set_2d_scale(SCREEN_SCALE);
    window_create("Heart Sprite Editor", SCREEN_WIDTH, SCREEN_HEIGHT, FULLSCREEN, 0);

    vertex_array_buffer_quad_create();
    shader = shader_load("assets/shaders/vert.frag", "assets/shaders/quad.frag");
    glUniform3f(glGetUniformLocation(shader, "resolution"), SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_SCALE);
    glUniform4f(glGetUniformLocation(shader, "camera"), 0.0f, 0.0f, 1.0f, 0.0f);

    bitmap_t* bitmap;
    if (strcmp(sourcefile, "$")) bitmap = bitmap_load(argv[1]);
    else bitmap = bitmap_color(width, height, pixel(255, 255, 255, 255));
    cursor = pixel(0, 0, 0, 255);

    bitmap_t* black = bitmap_color(16, 16, pixel(0, 0, 0, 255));
    bitmap_t* red = bitmap_color(16, 16, pixel(255, 0, 0, 255));
    bitmap_t* green = bitmap_color(16, 16, pixel(0, 255, 0, 255));
    bitmap_t* blue = bitmap_color(16, 16, pixel(0, 0, 255, 255));
    bitmap_t* violet = bitmap_color(16, 16, pixel(255, 0, 255, 255));
    bitmap_t* yellow = bitmap_color(16, 16, pixel(255, 255, 0, 255));
    bitmap_t* white = bitmap_color(16, 16, pixel(255, 255, 255, 255));
    bitmap_t* alpha = bitmap_color(16, 16, pixel(0, 0, 0, 0));
    
    vec2 mouse;
    float scale = 1.0f, rot = 0.0f;

    float last_time = time_start_timer(), delta_time;
    screen_color(0.5f, 0.5f, 0.5f, 0.5f);
    while(window_is_open()) {
        screen_clear();
        delta_time = time_get_delta_time(&last_time);

        mouse_get_position(&mouse.x, &mouse.y);
        moux = mouse.x;
        mouy = mouse.y;
        mx = (mouse.x - SCREEN_XSCALE / 2)/scale + SCREEN_XSCALE / 2;
        my = (mouse.y - SCREEN_YSCALE / 2)/scale + SCREEN_YSCALE / 2;
        //printf("%d, %d\n", mx, my);

        if (keyboard_down(GLFW_KEY_Z)) {
            if (keyboard_down(GLFW_KEY_RIGHT)) cursor.r ++;
            if (keyboard_down(GLFW_KEY_LEFT)) cursor.r --;
        }

        if (keyboard_down(GLFW_KEY_X)) {
            if (keyboard_down(GLFW_KEY_RIGHT)) cursor.g ++;
            if (keyboard_down(GLFW_KEY_LEFT)) cursor.g --;
        }

        if (keyboard_down(GLFW_KEY_C)) {
            if (keyboard_down(GLFW_KEY_RIGHT)) cursor.b ++;
            if (keyboard_down(GLFW_KEY_LEFT)) cursor.b --;
        }

        if (keyboard_down(GLFW_KEY_V)) {
            if (keyboard_down(GLFW_KEY_RIGHT)) {
                cursor.r ++;
                cursor.g ++;
                cursor.b ++;
            }
            if (keyboard_down(GLFW_KEY_LEFT)) {
                cursor.r --;
                cursor.g --;
                cursor.b --;
            }
        }

        if (keyboard_down(GLFW_KEY_UP)) scale += delta_time * 10.0f;
        if (keyboard_down(GLFW_KEY_DOWN)) scale -= delta_time * 10.0f;
        if (keyboard_pressed(GLFW_KEY_ESCAPE)) break;
        if (keyboard_pressed(GLFW_KEY_S)) scale = 1;
        if (keyboard_pressed(GLFW_KEY_P)) png_file_write(filename, (unsigned char*)bitmap->pixels, bitmap->width, bitmap->height);
        if (keyboard_pressed(GLFW_KEY_R)) {
            bitmap_free(bitmap);
            bitmap = bitmap_color(width, height, pixel(255, 255, 255, 255));
        }
        if (keyboard_pressed(GLFW_KEY_A)) set_bitmap_to_alpha(bitmap);
        if (mx >= 0 && mx < width && my >= 0 && my < height) {
            if (keyboard_pressed(GLFW_KEY_F)) flood_fill(bitmap, mx, my, pixel_at(bitmap, mx, my), &cursor);
        }
        //Draw 
        draw_trans_bitmap(bitmap, SCREEN_XSCALE/2, SCREEN_YSCALE/2, scale, rot);
        draw_transf_bitmap(black, SCREEN_XSCALE - 12, SCREEN_YSCALE - 32);
        draw_transf_bitmap(white, SCREEN_XSCALE - 32, SCREEN_YSCALE - 32);
        draw_transf_bitmap(red, SCREEN_XSCALE - 12, SCREEN_YSCALE - 64);
        draw_transf_bitmap(yellow, SCREEN_XSCALE - 32, SCREEN_YSCALE - 64);
        draw_transf_bitmap(blue, SCREEN_XSCALE - 12, SCREEN_YSCALE - 96);
        draw_transf_bitmap(green, SCREEN_XSCALE - 32, SCREEN_YSCALE - 96);
        draw_transf_bitmap(violet, SCREEN_XSCALE - 12, SCREEN_YSCALE - 128);
        draw_transf_bitmap(alpha, SCREEN_XSCALE - 32, SCREEN_YSCALE - 128);
        screen_refresh();
    }
    bitmap_free(bitmap);
    coreExit();
    return 0;
}
