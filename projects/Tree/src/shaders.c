#include "Tree.h"

#define FILE_SHADER_VERT_TEXTURE "assets/shaders/texture.vert.glsl"
#define FILE_SHADER_FRAG_TEXTURE "assets/shaders/texture.frag.glsl"
#define FILE_SHADER_VERT_FONT "assets/shaders/font.vert.glsl"
#define FILE_SHADER_FRAG_FONT "assets/shaders/font.frag.glsl"
#define FILE_SHADER_VERT_COLOR "assets/shaders/vert.frag"
#define FILE_SHADER_FRAG_COLOR "assets/shaders/quad.frag"
#define FILE_SHADER_VERT_FRAMEBUFFER "assets/shaders/framebufferv.frag"
#define FILE_SHADER_FRAG_FRAMEBUFFER "assets/shaders/framebufferf.frag"
#define FILE_SHADER_VERT_MESH "assets/shaders/treeTexture.vs"
#define FILE_SHADER_FRAG_MESH "assets/shaders/treeTexture.fs"

static float white[] = {1.0f, 1.0f, 1.0f, 1.0f};
static float cam[] = {0.0f, 0.0f, 1.0f, 0.0f};

unsigned int shaderLoadTexture()
{
    unsigned int shader = shader_load(FILE_SHADER_VERT_TEXTURE, FILE_SHADER_FRAG_TEXTURE);
    
    shader_set_uniform(shader, 3, "resolution", &viewport.x);
    shader_set_uniform(shader, 4, "camera", &cam[0]);
    shader_set_uniform(shader, 2, "scale_rot", &cam[2]);
    shader_set_uniform(shader, 4, "color", &white[0]);

    return shader;
}

unsigned int shaderLoadFont()
{
    unsigned int shader = shader_load(FILE_SHADER_VERT_FONT, FILE_SHADER_FRAG_FONT);
    
    shader_set_uniform(shader, 3, "resolution", &viewport.x);
    shader_set_uniform(shader, 4, "camera", &cam[0]);
    shader_set_uniform(shader, 4, "scale_rot", &cam[0]);
    shader_set_uniform(shader, 4, "color", &white[0]);

    return shader;
}

unsigned int shaderLoadColor()
{
    unsigned int shader = shader_load(FILE_SHADER_VERT_COLOR, FILE_SHADER_FRAG_COLOR);

    shader_set_uniform(shader, 3, "resolution", &viewport.x);
    shader_set_uniform(shader, 4, "camera", &cam[0]);
    shader_set_uniform(shader, 2, "scale_rot", &cam[2]);
    shader_set_uniform(shader, 4, "color", &white[0]);
    return shader;
}

unsigned int shaderLoadFramebuffer()
{
    return shader_load(FILE_SHADER_VERT_FRAMEBUFFER, FILE_SHADER_FRAG_FRAMEBUFFER);
}

unsigned int shaderLoadMesh()
{
    mat4 m = ortho(0.0f, (float)(viewport.x / viewport.z), 0.0f, (float)(viewport.y / viewport.z));
    unsigned int shader = shader_load(FILE_SHADER_VERT_MESH, FILE_SHADER_FRAG_MESH);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &m.data[0][0]);
    shader_set_uniform(shader, 4, "color", &white[0]);
    shader_set_uniform(shader, 4, "camera", &cam[0]);
    shader_set_uniform(shader, 3, "resolution", &viewport.x);
    return shader;
}
