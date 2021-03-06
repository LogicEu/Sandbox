#include <Imagine/Imagine.h>
#include <Fract/Fract.h>

#define MAX_RAY_STEP 100
#define MAX_RAY_DIST 100
#define SURFACE_DIST 0.1f

#define CLMPF(x) clampf(0.0f, 1.0f, x)

float minf(float a, float b)
{
    return a * (a <= b) + b * (b < a);
}

float scene_dist(vec3 position)
{
    //sphere -> .xyz = pos / .w = radius
    vec4 s = {0.0f, 2.0f, 5.0f, 1.0f};

    //distance from the center of sphere to position 
    float sphereDist = vec3_mag(vec3_sub(position, vec3_new(s.x, s.y, s.z))) - s.w;
    float planeDist = position.y;
    float dist = minf(sphereDist, planeDist);
    return dist;
}

float ray_march(vec3 origin, vec3 direction)
{
    float d = 0.0f;
    for (int i = 0; i < MAX_RAY_STEP; i++) {
        vec3 pos = vec3_add(origin, vec3_mult(direction, d));
        float dist = scene_dist(pos);
        d += dist;
        if (d > MAX_RAY_DIST || dist < SURFACE_DIST) break;
    }
    return d;
}

vec3 get_normal(vec3 pos)
{
    float dist = scene_dist(pos);
    vec2 e = {0.01, 0.0f};
    
    float x = scene_dist(vec3_sub(pos, vec3_new(e.x, e.y, e.y)));
    float y = scene_dist(vec3_sub(pos, vec3_new(e.y, e.x, e.y)));
    float z = scene_dist(vec3_sub(pos, vec3_new(e.y, e.y, e.x)));

    vec3 normal = vec3_sub(univec3(dist), vec3_new(x, y, z));
    return vec3_norm(normal);
}

float get_light(vec3 pos)
{
    vec3 light_pos = vec3_new(1.0f, 5.0f, 2.0f);
    vec3 l = vec3_norm(vec3_sub(light_pos, pos));
    vec3 n = get_normal(pos);
    float dif = clampf(vec3_dot(n, l), 0.0f, 1.0f);
    float d = ray_march(vec3_add(pos, vec3_mult(n, SURFACE_DIST)), l);
    if (d < vec3_mag(vec3_sub(light_pos, pos))) dif *= 0.1f;
    return dif;
}

int main(void)
{
    int width = 800, height = 600, channels = 4;
    bmp_t bmp = bmp_new(width, height, channels);
    
    vec2 res = {(float)width, (float)height}; 
    vec3 origin = {0.0f, 2.0f, -5.0f};

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            vec2 coord = {(float)x, (float)y};
            vec2 uv = vec2_div(vec2_sub(coord, vec2_mult(res, 0.5f)), res.y);
            
            vec3 dir = vec3_norm(vec3_new(uv.x, uv.y, 1.0f));
            float d = ray_march(origin, dir);
            vec3 pos = vec3_add(origin, vec3_mult(dir, d));
            float dif = get_light(pos); 

            color_t color = {CLMPF(0.1f + dif), CLMPF(0.1f + dif), CLMPF(0.1f + dif), 1.0f};
            pixel_t px = ctop(color);
            memcpy(px_at(&bmp, x, y), &px, sizeof(pixel_t));
        }
    }

    char* path = "output.gif";
    char op[256];
    bmp_t img = bmp_flip_vertical(&bmp);
    bmp_write(path, &img);
    bmp_free(&bmp);
    bmp_free(&img);
#ifdef __APPLE__
    strcpy(op, "open ");
#else
    strcpy(op, "xdg-open ");
#endif
    strcat(op, path);
    system(op);
    return 0;
}
