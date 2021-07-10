#include <Universe/Universe.h>

wxIcon wxIconCreate(texture_t texture, vec2 position, float scale)
{
    wxIcon icon;
    icon.texture = texture;
    icon.frame = wxFrameCreate(position, scale);
    return icon;
}

void wxIconUpdate(wxIcon* f, vec2 mouse, bool pressed)
{
    wxFrameUpdate(&f->frame, mouse, pressed);
}

void wxIconDraw(wxIcon* icon)
{
    wxFrame* f = &icon->frame;
    wxFrameDraw(f);
    
    texture_t t = tFrame, tex = icon->texture;
    vec2 pos = f->position;

    unsigned int k = tex.width * (tex.width >= tex.height) + tex.height * (tex.height > tex.width);
    vec2 off = {inverse_lerpf(0.0f, (float)k, (float)tex.width), inverse_lerpf(0.0f, (float)k, (float)tex.height)};

    float w = (float)t.width * 0.4f * f->scale * off.x;
    float h = (float)t.height * 0.4f * f->scale * off.y;

    float rect[] = {
        pos.x - w, pos.y + h,
        pos.x + w, pos.y - h
    };

    float vertices[] = {
        rect[0], rect[1], 0.0f, 0.0f,
        rect[0], rect[3], 0.0f, 1.0f,
        rect[2], rect[3], 1.0f, 1.0f,
        rect[0], rect[1], 0.0f, 0.0f,
        rect[2], rect[3], 1.0f, 1.0f,
        rect[2], rect[1], 1.0f, 0.0f
    };

    glBindVertexArray(textureVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textureVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindTexture(GL_TEXTURE_2D, tex.id);
    glUniform4f(glGetUniformLocation(textureShader, "color"), 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}