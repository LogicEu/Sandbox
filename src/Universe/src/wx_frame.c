#include <Universe/Universe.h>

wxFrame wxFrameCreate(vec2 position, float scale)
{
    wxFrame f;
    f.state = WIDGET_UNSELECTED;
    f.position = position;
    f.scale = scale;
    return f;
}

void wxFrameUpdate(wxFrame* f, vec2 mouse, bool pressed)
{
    texture_t t = tFrame;
    rect_t r = {f->position.x, f->position.y, (float)t.width * f->scale, (float)t.height * f->scale};
    if (!point_meeting(mouse, r)) {
        f->state = WIDGET_UNSELECTED;
        return;
    }
    
    f->state = WIDGET_HOVER;
    if (pressed) f->state = WIDGET_SELECTED;
}

void wxFrameDraw(wxFrame* f)
{
    texture_t t = tFrame;
    color_t c = globalTheme.primary[f->state];
    vec2 pos = f->position;

    glUseProgram(textureShader);
    glBindTexture(GL_TEXTURE_2D, t.id);
    glUniform4f(glGetUniformLocation(textureShader, "color"), c.r, c.g, c.b, c.a);
    glBindVertexArray(textureVAO);

    float rect[] = {
        pos.x - (float)t.width * 0.5f * f->scale, pos.y + (float)t.height * 0.5f * f->scale,
        pos.x + (float)t.width * 0.5f * f->scale, pos.y - (float)t.height * 0.5f * f->scale
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