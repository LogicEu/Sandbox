#include <Dwarf/Dwarf.h>

void light_set(unsigned int shader, vec4 light)
{
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "Light"), light.x, light.y, light.z, light.w);
}

void draw_pixel_raw(unsigned int shader, pixel_t p, float x, float y)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, x, y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), 1.0f / (float)core_get_2d_scale(), 0.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void draw_pixel_raw_transformed(unsigned int shader, pixel_t p, float x, float y, float origin_x, float origin_y, float scale, float rot)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, origin_x, origin_y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), scale / (float)core_get_2d_scale(), rot, 1.0f, 0.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void draw_pixel(unsigned int shader, pixel_t p, float x, float y)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, x, y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), 1.0f, 0.0f, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
}

void draw_proto_pixel(unsigned int shader, pixel_t p, float x, float y, float origin_x, float origin_y, float scale, float rot)
{
    color_t c = ptoc(p);
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, origin_x, origin_y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), scale, rot, 1.0f, 1.0f);
    glUniform4f(glGetUniformLocation(shader, "color"), c.r, c.g, c.b, c.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_rect(unsigned int shader, rect_t rect, color_t color)
{
    glUseProgram(shader);
    glUniform4f(glGetUniformLocation(shader, "trans"), rect.x, rect.y, rect.x, rect.y);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), 1.0f, 0.0f, rect.w, rect.h);
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void draw_texture(unsigned int shader, texture_t* texture, float x, float y, float scale, float rot)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, texture->width, texture->height);
    glUniform2f(glGetUniformLocation(shader, "scale_rot"), scale, rot);
    glUniform4f(glGetUniformLocation(shader, "color"), 1., 1., 1., 1.);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_texture_color(unsigned int shader, texture_t* texture, color_t color, float x, float y, float scale, float rot)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, texture->width, texture->height);
    glUniform2f(glGetUniformLocation(shader, "scale_rot"), scale, rot);
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_texture_transformed(unsigned int shader, texture_t* texture, color_t color, float x, float y, float width, float height, float scale, float rot)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, width, height);
    glUniform2f(glGetUniformLocation(shader, "scale_rot"), scale, rot);
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_font_char(unsigned int shader, unsigned int font_id, color_t color, float origin_x, float origin_y, float x, float y, float width, float height, float scale, float rot)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, font_id);
    glUniform4f(glGetUniformLocation(shader, "trans"), x, y, width, height);
    glUniform4f(glGetUniformLocation(shader, "scale_rot"), scale, rot, origin_x, origin_y);
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_framebuffer(unsigned int shader, texture_t* texture)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_cube(unsigned int shader, mat4 mvp, color_t color)
{
    glUseProgram(shader);
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp.data[0][0]);
    glUniform4f(glGetUniformLocation(shader, "color"), color.r, color.g, color.b, color.a);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void draw_cube_texture(unsigned int shader, mat4 mvp, texture_t* texture)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp.data[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_model(unsigned int shader, model_t* model, texture_t* texture, mat4 projection, mat4 view, mat4 m, vec3 pos)
{
    glUseProgram(shader);
    glBindVertexArray(model->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform3f(glGetUniformLocation(shader, "viewPos"), pos.x, pos.y, pos.z);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &m.data[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, model->size);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_skybox(unsigned int shader, skybox_t* skybox, mat4 mvp)
{
    glDepthMask(GL_FALSE);
    glUseProgram(shader);
    glBindVertexArray(skybox->VAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->cubemap.id);
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &mvp.data[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
}

void draw_mesh(unsigned int shader, unsigned int id, unsigned int size, texture_t* texture, mat4 projection, mat4 view, mat4 m, vec3 pos, unsigned int mode)
{
    glUseProgram(shader);
    glBindVertexArray(id);
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glUniform3f(glGetUniformLocation(shader, "viewPos"), pos.x, pos.y, pos.z);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &m.data[0][0]);
    glDrawArrays(mode, 0, size);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_vmesh(unsigned int shader, unsigned int id, unsigned int size, texture_t* t, mat4 projection, mat4 view, mat4 m, vec3 pos, unsigned int mode)
{
    glUseProgram(shader);
    glBindTexture(GL_TEXTURE_2D, t->id);
    glBindVertexArray(id);
    glUniform3f(glGetUniformLocation(shader, "viewPos"), pos.x, pos.y, pos.z);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &view.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &projection.data[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &m.data[0][0]);
    glDrawElements(mode, size, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

//-------------------------------//
//     -> Draw Data Types  <-    //
//-------------------------------//

void draw_string(unsigned int shader, const char* text, font_t* chs, float x, float y, float scale, float rot, color_t color)
{
    const unsigned int size = strlen(text);
    float xpos[size], ypos[size], w[size], h[size];
    for (unsigned int i = 0; i < size; i++) {
        font_t ch = *(chs + text[i]);
        xpos[i] = x + ch.bearing_x * scale;
        ypos[i] = y + (ch.size_y + ch.bearing_y);
        w[i] = ch.size_x * scale;
        h[i] = ch.size_y * scale;
        x += (ch.advance >> 6) * scale; 
        draw_font_char(shader, (chs + text[i])->id, color, x, y, xpos[i], ypos[i], w[i], h[i], scale, rot);
    }
}

void draw_font_string(unsigned int shader, const char* text, font_t* chs, float x, float y, float scale, float rot, color_t color)
{
    const float xx = x, yy = y;
    const unsigned int size = strlen(text);
    float xpos[size], ypos[size], w[size], h[size], acc = x;
    for (unsigned int i = 0; i < size; i++) {
        font_t ch = *(chs + text[i]);
        xpos[i] = x + ch.bearing_x * scale;
        ypos[i] = y - (ch.size_y - ch.bearing_y);
        w[i] = ch.size_x * scale;
        h[i] = ch.size_y * scale;
        x += (ch.advance >> 6) * scale; 
        acc += ((ch.advance >> 6) + ch.bearing_x + ch.size_x);
    }

    float hlength = (acc - xx) * scale / 4.0f;
    for (unsigned int i = 0; i < size; i++) {
        draw_font_char(shader, (chs + text[i])->id, color, xx, yy, xpos[i] - hlength, ypos[i], w[i], h[i], 1., rot);
    }
}

void draw_bitmap(unsigned int shader, bitmap_t* bitmap, float bitmap_x, float bitmap_y, float scale, float rot)
{
    for (uint32_t y = 0; y < bitmap->height; y++) {
        for (uint32_t x = 0; x < bitmap->width; x++) {
            float xpos = (float)x + bitmap_x - (float)bitmap->width / 2.0f;
            float ypos = ((float)bitmap->height - (float)y) + bitmap_y - (float)bitmap->height / 2.0f;
            draw_proto_pixel(shader, *pixel_at(bitmap, x, y), xpos, ypos, bitmap_x, bitmap_y, scale, rot);
        }
    }
}

void draw_sprite_frame(unsigned int shader, sprite_t* sprite, unsigned int frame, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot)
{
    draw_texture_transformed(shader, &sprite->textures[frame], unicolor(1.0f), sprite_x, sprite_y, width_scale, height_scale, scale, rot);
}

void draw_sprite_static(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot)
{
    draw_sprite_frame(shader, sprite, sprite->current_frame, sprite_x, sprite_y, width_scale, height_scale, scale, rot);
}

void draw_sprite(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float scale, float rot)
{
    draw_sprite_static(shader, sprite, sprite_x, sprite_y, 1.0f, 1.0f, scale, rot);
    sprite_frame_update(sprite);
} 

void draw_sprite_transformed(unsigned int shader, sprite_t* sprite, float sprite_x, float sprite_y, float width_scale, float height_scale, float scale, float rot)
{
    draw_sprite_static(shader, sprite, sprite_x, sprite_y, width_scale, height_scale, scale, rot);
    sprite_frame_update(sprite);
}

//-------------------------------//
//     -> Draw Entities    <-    //
//-------------------------------//

void draw_gizmos(unsigned int shader, rect_t r, float scale, float rot)
{
    const unsigned int width = (unsigned int)(r.w * 2.0f);
    const unsigned int height = (unsigned int)(r.h * 2.0f);
    pixel_t pixel = {255, 0, 0, 255};
    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            if (x == 0 || x == width - 1 || y == 0 || y == width -1) {
                float xpos = (float)x + r.x - r.w;
                float ypos = (float)(height - y) + r.y - r.h;
                draw_proto_pixel(shader, pixel, xpos, ypos, r.x, r.y, scale, rot);
            } else continue;
        }
    }
}

/*void draw_entity(entity_t* entity) 
{
    if (entity != NULL && entity->sprite != NULL) {
        draw_sprite_transformed(entity->sprite, entity->rect.x, entity->rect.y, entity->rect.w * 2.0f, entity->rect.h * 2.0f, entity->scale, entity->rot);
    }
}

void draw_entity_list(list_t* list)
{
    if (list->first != NULL) {
        node_t* node = list->first;
        entity_t* entity = (entity_t*)node->data;
        draw_sprite_transformed(entity->sprite, entity->rect.x, entity->rect.y, entity->rect.w * 2.0f, entity->rect.h * 2.0f, entity->scale, entity->rot);
        while (node != NULL) {
            entity = (entity_t*)node->data;
            draw_sprite_static(entity->sprite, entity->rect.x, entity->rect.y, entity->rect.w * 2.0f, entity->rect.h * 2.0f, entity->scale, entity->rot);
            node = node->next;
        }
    }
}

void draw_entity_gizmos(entity_t* entity)
{
    draw_gizmos(entity->rect, entity->scale, entity->rot);
}

void draw_entity_list_gizmos(list_t* list)
{
    if (list->first != NULL) {
        node_t* node = list->first;
        while (node != NULL) {
            draw_entity_gizmos((entity_t*)node->data);
            node = node->next;
        }
    }
}*/

void vertex_element_buffer(unsigned int id, array_t* vertices, array_t* indices)
{
    if (vertices != NULL && indices != NULL) {
        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);  
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->used * indices->bytes, indices->data, GL_STATIC_DRAW);

        // Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)0);
        // Uvs
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, uv));
        // Normals
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex_t), (void*)offsetof(vertex_t, normal));
    }
}

void vertex_array_buffer(unsigned int id, array_t* array, unsigned int attribute)
{
    if (array == NULL) return;
    unsigned int buffer;
    glBindVertexArray(id);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, array->used * array->bytes, array->data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(attribute);
    glVertexAttribPointer(
        attribute,                           // attribute 
        array->bytes / sizeof(float),        // size
        GL_FLOAT,                            // type
        GL_FALSE,                            // normalized?
        0,                                   // stride
        (void*)0                             // array buffer offset
    );
}

void vertex_element_buffer_obj(unsigned int id, array_t* vertices, array_t* indices, obj_flag type)
{
    if (vertices != NULL && indices != NULL) {
        unsigned int VBO, EBO;
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(id);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices->used * vertices->bytes, vertices->data, GL_STATIC_DRAW);  
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->used * indices->bytes, indices->data, GL_STATIC_DRAW);
        

        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)0);
        if (type == OBJ_VTN) {
            glEnableVertexAttribArray(1);	
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, uv));
        }
        if (type == OBJ_VN || type == OBJ_VTN) {
            glEnableVertexAttribArray(2);	
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertices->bytes, (void*)offsetof(vertex_t, normal));
        }
    }
}

void mesh_bind(unsigned int id, mesh_t* mesh)
{
    if (mesh->vertices != NULL) vertex_array_buffer(id, mesh->vertices, 0);
    if (mesh->uvs != NULL) vertex_array_buffer(id, mesh->uvs, 1);
    if (mesh->normals != NULL) vertex_array_buffer(id, mesh->normals, 2);
}

void vmesh_bind(unsigned int id, vmesh_t* mesh)
{
    if (mesh != NULL) {
        vertex_element_buffer_obj(id, mesh->vertices, mesh->indices, mesh->type);
    }
}

void model_bind(model_t* model, array_t* vertices, array_t* uvs, array_t* normals)
{
    vertex_array_buffer(model->id, vertices, 0);
    vertex_array_buffer(model->id, uvs, 1);
    vertex_array_buffer(model->id, normals, 2);
}
