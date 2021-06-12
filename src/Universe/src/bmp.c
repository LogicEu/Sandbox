#include <Universe/Universe.h>

void bmpDraw(bmp_t* bmp, float xPos, float yPos, float scale)
{
    glUseProgram(widgetShader);
    glBindVertexArray(widgetVAO);

    float wPos = xPos - (float)bmp->width * scale * 0.5f;
    float hPos = yPos - (float)bmp->height * scale * 0.5f;

    for (unsigned int y = 0; y < bmp->width; y++) {
        for (unsigned int x = 0; x < bmp->height; x++) {
            color_t c = ptoc(*(pixel_t*)px_at(bmp, x, y));

            float dx = wPos + (float)x * scale;
            float dy = hPos + (float)y * scale;

            float rect[] = {
                dx, dy,
                dx + scale, dy + scale
            };

            float vertices[] = {
                rect[0], rect[1],
                rect[0], rect[3],
                rect[2], rect[3],
                rect[0], rect[1],
                rect[2], rect[3],
                rect[2], rect[1]
            };

            glUniform4f(glGetUniformLocation(widgetShader, "color"), c.r, c.g, c.b, c.a);
            glBindBuffer(GL_ARRAY_BUFFER, widgetVBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }
    glBindVertexArray(0);
}