#include <Universe/Universe.h>

wxRect wxRectCreate(rect_t rect, color_t color)
{
    wxRect r;
    r.rect = rect;
    r.color = color;
    return r;
}

void wxRectDraw(wxRect _rect)
{
    color_t color = _rect.color;
    rect_t r = _rect.rect;

    glUseProgram(widgetShader);
    glUniform4f(glGetUniformLocation(widgetShader, "color"), color.r, color.g, color.b, color.a);
    glBindVertexArray(widgetVAO);

    float rect[] = {
        r.x - r.w * 0.5f, r.y - r.h * 0.5f,
        r.x + r.w * 0.5f, r.y + r.h * 0.5f
    };

    float vertices[] = {
        rect[0], rect[1],
        rect[0], rect[3],
        rect[2], rect[3],
        rect[0], rect[1],
        rect[2], rect[3],
        rect[2], rect[1]
    };

    glBindBuffer(GL_ARRAY_BUFFER, widgetVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
