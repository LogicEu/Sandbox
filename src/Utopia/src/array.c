#include <Utopia/Utopia.h>

/*
----------------------------
 -> Dynamic Generic Array <- 
----------------------------
*/

array_t array(unsigned int size, unsigned int bytes)
{
    array_t array;
    array.data = malloc(size * bytes);
    array.bytes = bytes;
    array.size = size;
    array.used = 0;
    return array;
}

array_t* array_new(unsigned int size, unsigned int bytes)
{
    array_t* array = (array_t*)malloc(sizeof(array_t));
    array->data = malloc(size * bytes);
    array->bytes = bytes;
    array->size = size;
    array->used = 0;
    return array;
}

unsigned int array_is_empty(array_t* array)
{
    return array->used == 0;
}

unsigned int array_is_full(array_t* array)
{
    return array->used >= array->size;
}

void array_resize(array_t* array, unsigned int new_size)
{
    if (new_size <= array->used) return;
    array->size = new_size;
    array->data = realloc(array->data, array->size * array->bytes);
}

void array_cut(array_t* array)
{
    array->size = array->used;
    array->data = realloc(array->data, array->size * array->bytes);
}

void* array_index(array_t* array, unsigned int index)
{   
    return (void*)((char*)array->data + index * array->bytes);
}

void array_push(array_t* array, void* data)
{
    if (array->data == NULL) array->data = malloc(array->size * array->bytes);
    if (array_is_full(array)) array_resize(array, array->size * 2);
    memcpy(array_index(array, array->used++), data, array->bytes);
}

void array_remove(array_t* array, unsigned int index)
{
    if (index >= array->used) return;
    char* ptr = (char*)array_index(array, index);
    for (char* end = (char*)array->data + array->used * array->bytes - array->bytes; ptr != end; ptr += array->bytes) {
        memcpy(ptr, ptr + array->bytes, array->bytes);
    }
    //bzero(ptr, array->bytes);
    array->used--;
}

int array_find(array_t* array, void* data)
{
    int index = 0;
    char* ptr = (char*)array->data;
    for (char* end = ptr + array->used * array->bytes; ptr != end; ptr += array->bytes) {
        if (!memcmp(ptr, data, array->bytes)) return index;
        index++;
    }
    return -1;
}

void* array_pop(array_t* array)
{
    if (array_is_empty(array)) return NULL;
    return array_index(array, --array->used);
}

void array_free(array_t* array)
{
    if (array->data == NULL) return;
    free(array->data);
    array->data = NULL;
    array->used = 0;
}

void array_destroy(array_t* array)
{
    if (array == NULL) return;
    array_free(array);
    free(array);
}
