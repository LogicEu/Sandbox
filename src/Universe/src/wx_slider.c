#include <Universe/Universe.h>

wxSlider wxSliderCreate(vec2 position, float scale, bool rot)
{
    wxSlider s;
    s.lerp = 0.0f;
    s.position = position;
    s.selected = false;
    s.scale = scale;
    s.rot = rot;
    return s;
}

void wxSliderUpdate(wxSlider* s, vec2 mouse, bool pressed)
{
    texture_t t = tSlider[1];
    texture_t tt = tSlider[0];
    float h = (float)tt.width * s->scale;


    rect_t r;
    if (!s->rot) r = rect_new(s->position.x - h * 0.5f + h * s->lerp, s->position.y, (float)t.width * s->scale, (float)t.height * s->scale);
    else r = rect_new(s->position.x, s->position.y - h * 0.5f + h * s->lerp, (float)t.width * s->scale, (float)t.height * s->scale);
    
    bool hover = point_meeting(mouse, r);
    if (!pressed) s->selected = false;
    else if (hover) s->selected = true;
    
    if (s->selected) {
        if (!s->rot) {
            float f[2] = {s->position.x - h * 0.5f, s->position.x + h * 0.5f};
            float x = clampf(mouse.x , f[0], f[1]);
            s->lerp = inverse_lerpf(f[0], f[1], x);
        } else {
            float f[2] = {s->position.y - h * 0.5f, s->position.y + h * 0.5f};
            float y = clampf(mouse.y, f[0], f[1]);
            s->lerp = inverse_lerpf(f[0], f[1], y);
        }
    }
}

static void drawImg(texture_t t, color_t c, vec2 pos, float scale, bool rot)
{
    glBindTexture(GL_TEXTURE_2D, t.id);
    glUniform4f(glGetUniformLocation(textureShader, "color"), c.r, c.g, c.b, c.a);
    glBindVertexArray(textureVAO);

    float rect[4]; 
    if (!rot) {
        rect[0] = pos.x - (float)t.width * 0.5f * scale;
        rect[1] = pos.y - (float)t.height * 0.5f * scale;
        rect[2] = pos.x + (float)t.width * 0.5f * scale;
        rect[3] = pos.y + (float)t.height * 0.5f * scale;

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
    } else {
        rect[0] = pos.x - (float)t.height * 0.5f * scale;
        rect[1] = pos.y - (float)t.width * 0.5f * scale;
        rect[2] = pos.x + (float)t.height * 0.5f * scale;
        rect[3] = pos.y + (float)t.width * 0.5f * scale;

        float vertices[] = {
            rect[0], rect[1], 0.0f, 1.0f,
            rect[0], rect[3], 1.0f, 1.0f,
            rect[2], rect[3], 1.0f, 0.0f,
            rect[0], rect[1], 0.0f, 1.0f,
            rect[2], rect[3], 1.0f, 0.0f,
            rect[2], rect[1], 0.0f, 0.0f
        };

        glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void wxSliderDraw(wxSlider* s)
{
    glUseProgram(textureShader);
    drawImg(tSlider[0], globalTheme.terciary[0], s->position, s->scale, s->rot);

    float h = (float)tSlider[0].width * s->scale;
    vec2 pos;
    if (!s->rot) pos = vec2_new(s->position.x - h * 0.5f + h * s->lerp, s->position.y);
    else pos = vec2_new(s->position.x, s->position.y - h * 0.5f + h * s->lerp);
    drawImg(tSlider[1], globalTheme.primary[1], pos, s->scale, s->rot);
}