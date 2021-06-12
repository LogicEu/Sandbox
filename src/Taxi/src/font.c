#include <Taxi/Taxi.h>
#include <ft2build.h>
#include FT_FREETYPE_H

/*
-------------------
 -> Font Handler <- 
-------------------
*/

font_t* font_load(const char* path, unsigned int size)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft)) {
        printf("ERROR::FREETYPE: Could not init FreeType Library\n");
        exit(-1);
    }

    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face)) {
        printf("ERROR::FREETYPE: Failed to load font '%s'\n", path);  
        exit(-1);
    }
    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    font_t* glyphs = (font_t*)malloc(sizeof(font_t) * 128);
    for (unsigned char c = 0; c < 128; c++) {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            printf("ERROR::FREETYTPE: Failed to load Glyph\n");
        }

        glGenTextures(1, &((glyphs + c)->id));
        glBindTexture(GL_TEXTURE_2D, (glyphs + c)->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 
            face->glyph->bitmap.width, face->glyph->bitmap.rows, 0,
            GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        (glyphs + c)->size_x = face->glyph->bitmap.width;
        (glyphs + c)->size_y = face->glyph->bitmap.rows;
        (glyphs + c)->bearing_x = face->glyph->bitmap_left;
        (glyphs + c)->bearing_y = face->glyph->bitmap_top;
        (glyphs + c)->advance = face->glyph->advance.x;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    return glyphs;
}