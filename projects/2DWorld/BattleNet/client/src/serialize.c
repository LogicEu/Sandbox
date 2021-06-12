#include "../BNGame.h"

array_t* buffer_load_array(const char* path, unsigned int bytes)
{
    array_t* array = (array_t*)malloc(sizeof(array_t));
    size_t size;
    array->data = buffer_load_bin(path, &size);
    array->size = array->used = size / bytes;
    array->bytes = bytes;
    return array;
}

void buffer_write_array(const char* path, array_t* array)
{
    buffer_write_bin(path, array->data, array->used * array->bytes);
}