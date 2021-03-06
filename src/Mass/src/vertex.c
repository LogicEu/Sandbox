#include <Mass/Mass.h>

/*
--------------------------
 -> 3D Vertex Data Type <- 
--------------------------
*/

vertex_t vertex_new(vec3 position, vec2 uv, vec3 normal)
{
    vertex_t v;
    v.position = position;
    v.uv = uv;
    v.normal = normal;
    return v;
}

void vertex_array_normalize_normals(vertex_t* v, const unsigned int size)
{
    for (vertex_t* end = v + size; v != end; v++) {
        v->normal = vec3_norm(v->normal);
    }
}

void vec3_normal_array_normalize(vec3* v, const unsigned int size)
{
    for (vec3* end = v + size; v != end; v++) {
        *v = vec3_norm(*v);
    }
}

array_t* vertex_array_by_index_array(array_t* index, array_t* buffer)
{
    array_t* ret = NULL;
    if (index->used > 0) {
        ret = array_new(index->used, buffer->bytes);
        for (unsigned int i = 0; i < index->used; i++) {
            unsigned int j = *(unsigned int*)array_index(index, i);
            array_push(ret, array_index(buffer, j - 1));
        }
    }
    array_free(index);
    array_free(buffer);
    return ret;
}