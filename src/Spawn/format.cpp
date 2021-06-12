#include <Spawn/Spawn.h>

void* buffer_load_fmt(const char* path, size_t *size, const char* fmt, unsigned int count, ...)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be loaded.\n", path);
        return NULL;
    }
    char* buffer = (char*)malloc(32);
    va_list args;
    va_start(args, count);
    while (1) {
        if (feof(file)) break;
        //vfscanf(file, fmt, va_arg(args, ))
    }
    fclose(file);
}

void buffer_read_fmt(const char* path, void* buffer, size_t* size, const char* fmt, unsigned int count, ...)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be read.\n", path);
        return;
    }
}

void buffer_write_fmt(const char* path, void* buffer, size_t size, const char* fmt, unsigned int count, ...)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be opened.\n", path);
        return;
    }
}