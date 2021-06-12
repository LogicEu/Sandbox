#include <Universe/Universe.h>

Font* globalFont;
texture_t tSwitch[2], tSlider[2];
Theme globalTheme;

bool defaultTheme = true, initSwitch = false, initSlider = false, initFont = false;
unsigned int widgetShader, fontShader, textureShader;
unsigned int fontVAO, fontVBO, widgetVAO, widgetVBO, textureVAO, textureVBO;
unsigned int scrWidth, scrHeight, scrScale, scrXscale, scrYscale;

static void vertexBuffersInit(unsigned int* VAO, unsigned int* VBO, unsigned int attribute, unsigned int stride)
{
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, VBO);
    glBindVertexArray(*VAO);
    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * stride, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(attribute, stride, GL_FLOAT, GL_FALSE, stride * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

static void universeCoordsSet(unsigned int width, unsigned int height, unsigned int scale)
{
    scrWidth = width;
    scrHeight = height;
    scrScale = scale;
    
    mat4 m = ortho(0.0f, (float)(width / scale), 0.0f, (float)(height / scale));

    glUseProgram(fontShader);
    glUniformMatrix4fv(glGetUniformLocation(fontShader, "projection"), 1, GL_FALSE, &m.data[0][0]);

    glUseProgram(widgetShader);
    glUniformMatrix4fv(glGetUniformLocation(widgetShader, "projection"), 1, GL_FALSE, &m.data[0][0]);

    glUseProgram(textureShader);
    glUniformMatrix4fv(glGetUniformLocation(textureShader, "projection"), 1, GL_FALSE, &m.data[0][0]);
}

void universeInit(unsigned int width, unsigned int height, unsigned int scale)
{   
    if (defaultTheme) {
        Theme t = {{
                {0.3f, 0.3f, 1.0f, 1.0f}, 
                {0.0f, 0.0f, 1.0f, 1.0f},
                {1.0f, 0.0f, 0.0f, 1.0f}
            }, {
                {1.0f, 0.3f, 0.3f, 1.0f}, 
                {1.0f, 0.0f, 0.0f, 1.0f},
                {0.0f, 0.0f, 1.0f, 1.0f}
            }, {
                {0.3f, 0.3f, 0.3f, 1.0f},
                {0.0f, 0.0f, 0.0f, 1.0f},
                {1.0f, 1.0f, 1.0f, 1.0f}
            }
        };
        globalTheme = t;
    }

    fontShader = shader_load("assets/shaders/text.vs", "assets/shaders/text.fs");
    widgetShader = shader_load("assets/shaders/widget.vs", "assets/shaders/widget.fs");
    textureShader = shader_load("assets/shaders/textures.vs", "assets/shaders/textures.fs");

    universeCoordsSet(width, height, scale);

    vertexBuffersInit(&fontVAO, &fontVBO, 10, 4);
    vertexBuffersInit(&textureVAO, &textureVBO, 10, 4);
    vertexBuffersInit(&widgetVAO, &widgetVBO, 11, 2);
}

void universeThemeSet(color_t primary[3], color_t secondary[3], color_t terciary[3])
{
    memcpy(globalTheme.primary, primary, sizeof(color_t) * 3);
    memcpy(globalTheme.secondary, secondary, sizeof(color_t) * 3);
    memcpy(globalTheme.terciary, terciary, sizeof(color_t) * 3);
    defaultTheme = false;
}

void universeSwitchSet(texture_t tActive, texture_t tInactive)
{
    tSwitch[0] = tInactive;
    tSwitch[1] = tActive;
    initSwitch = true;
}

void universeSliderSet(texture_t slider, texture_t marker)
{
    tSlider[0] = slider;
    tSlider[1] = marker;
    initSlider = true;
}

void universeFontSet(Font* font)
{
    globalFont = font;
    initFont = true;
}

Theme* universeThemeGet()
{
    return &globalTheme;
}

Font* universeFontGet()
{
    return globalFont;
}