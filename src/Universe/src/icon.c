#include <Universe/Universe.h>
#include <math.h>

Icon iconCreate(texture_t texture, vec2 position, float scale, float rotation)
{
    Icon icon;
    icon.color = color(1.0f, 1.0f, 1.0f, 1.0f);
    icon.texture = texture;
    icon.position = position;
    icon.scale = scale;
    icon.rotation = rotation;
    return icon;
}

void iconDraw(Icon* icon)
{
    vec2 pos = icon->position;
    float w = (float)icon->texture.width * 0.5f * icon->scale;
    float h = (float)icon->texture.height * 0.5f * icon->scale;

    vec2 p1 = vec2_add(pos, vec2_rotate(vec2_new(-w, -h), icon->rotation));
    vec2 p2 = vec2_add(pos, vec2_rotate(vec2_new(-w, h), icon->rotation));
    vec2 p3 = vec2_add(pos, vec2_rotate(vec2_new(w, h), icon->rotation));
    vec2 p4 = vec2_add(pos, vec2_rotate(vec2_new(w, -h), icon->rotation));

    float vertices[] = {
        p1.x, p1.y, 0.0f, 1.0f,
        p2.x, p2.y, 0.0f, 0.0f,
        p3.x, p3.y, 1.0f, 0.0f,
        p1.x, p1.y, 0.0f, 1.0f,
        p3.x, p3.y, 1.0f, 0.0f,
        p4.x, p4.y, 1.0f, 1.0f
    };

    glUseProgram(textureShader);
    glBindTexture(GL_TEXTURE_2D, icon->texture.id);
    glUniform4f(glGetUniformLocation(textureShader, "color"), icon->color.r, icon->color.g, icon->color.b, icon->color.a);
    glBindVertexArray(textureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindTexture(GL_TEXTURE_2D, icon->texture.id);
    glBindVertexArray(0);
}