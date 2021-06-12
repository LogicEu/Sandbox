#include <Mass/Mass.h>

/*
-----------------------------
 -> Useful Array Extension <- 
-----------------------------
*/

array_t* array_map(array_t* buffer)
{
    array_t* map = NULL;
    if (buffer != NULL) {
        array_t* map = array_new(256, buffer->bytes);
        char* p1 = buffer->data;
        for (char* end = p1 + buffer->used; p1 != end; p1 += buffer->bytes) {
            unsigned int found = 0;
            char* p2 = buffer->data;
            for (char* end = p2 + map->used; p2 != end; p2 += map->bytes) {
                if (!memcmp(p1, p2, buffer->bytes)){
                    found++;
                    break;
                }
            }
            if (!found) array_push(map, p1);
        }
        array_cut(map);
    }
    return map;
}

void array_map_indexed(array_t* buffer, array_t** map, array_t** indices)
{
    if (buffer != NULL) {
        *map = array_new(buffer->used, buffer->bytes);
        *indices = array_new(buffer->used, sizeof(unsigned int));
        char* p1 = buffer->data;
        for (char* end = p1 + buffer->used; p1 != end; p1 += buffer->bytes) {
            unsigned int j = 0, k = (*map)->used;
            char* p2 = buffer->data;
            for (char* end = p2 + (*map)->used; p2 != end; p2 += (*map)->bytes) {
                if (!memcmp(p1, p2, buffer->bytes)){
                    k = j;
                    break;
                }
                j++;
            }
            if (k == (*map)->used) array_push(*map, p1);
            array_push(*indices, &k);
        }
        array_cut(*map);
    }
}