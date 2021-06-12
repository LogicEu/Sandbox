#include <Spawn/Spawn.h>
#include <stdio.h>

void* buffer_load_bin(const char* path, size_t *size)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be loaded.\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    void* buffer = malloc(length + 1);
    fread(buffer, sizeof(char), length, file);
    *((char*)buffer + length) = '\0';
    fclose(file);
    *size = (size_t)length;
    printf("Succesfully loaded file '%s'.\n", path);
    return buffer;
}

void buffer_read_bin(const char* path, void* buffer, size_t* size)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be read.\n", path);
        return;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    fread(buffer, sizeof(char), length, file);
    *((char*)buffer + length) = '\0';
    fclose(file);
    *size = (size_t)length;
    printf("Succesfully read file '%s'.\n", path);
}

void buffer_write_bin(const char* path, void* buffer, size_t size)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Spawn Warning! - File '%s' could not be opened.\n", path);
        return;
    }
    fwrite(buffer, sizeof(char), size, file);
    fclose(file);
    printf("Succesfully writed file '%s'.\n", path);
}