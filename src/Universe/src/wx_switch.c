#include <Universe/Universe.h>

wxSwitch wxSwitchCreate(vec2 position, float scale)
{
    wxSwitch s;
    s.activated = false;
    s.position = position;
    s.scale = scale;
    return s;
}

void wxSwitchUpdate(wxSwitch* s, vec2 mouse, bool pressed)
{
    texture_t t = tSwitch[s->activated];
    rect_t r = {s->position.x, s->position.y, (float)t.width * s->scale, (float)t.height * s->scale};
    bool hover = point_meeting(mouse, r);
    if (hover && pressed) s->activated = !s->activated;
}

void wxSwitchDraw(wxSwitch* s)
{
    texture_t t = tSwitch[s->activated];
    color_t c = globalTheme.primary[s->activated + 1];
    vec2 pos = s->position;

    glUseProgram(textureShader);
    glBindTexture(GL_TEXTURE_2D, t.id);
    glUniform4f(glGetUniformLocation(textureShader, "color"), c.r, c.g, c.b, c.a);
    glBindVertexArray(textureVAO);

    float rect[] = {
        pos.x - (float)t.width * 0.5f * s->scale, pos.y - (float)t.height * 0.5f * s->scale,
        pos.x + (float)t.width * 0.5f * s->scale, pos.y + (float)t.height * 0.5f * s->scale
    };

    float vertices[] = {
        rect[0], rect[1], 0.0f, 0.0f,
        rect[0], rect[3], 0.0f, 1.0f,
        rect[2], rect[3], 1.0f, 1.0f,
        rect[0], rect[1], 0.0f, 0.0f,
        rect[2], rect[3], 1.0f, 1.0f,
        rect[2], rect[1], 1.0f, 0.0f
    };

    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}