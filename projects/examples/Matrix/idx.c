#include "idx.h"

int flip_int(uint8_t* data)
{
    uint8_t buff[sizeof(int)];
    for (int i = 0; i < sizeof(int); i++) {
        buff[i] = data[sizeof(int) - 1 - i];
    }
    return *(int*)&buff[0];
}

uint8_t* file_read(char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t* buffer = (uint8_t*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);
    return buffer;
}

uint8_t* idx3_header_read(uint8_t* buffer, int* count, int* width, int* height)
{
    int mark = 0;
    int magic_number = flip_int(&buffer[sizeof(int) * mark++]);
    *count = flip_int(&buffer[sizeof(int) * mark++]);
    *width = flip_int(&buffer[sizeof(int) * mark++]);
    *height = flip_int(&buffer[sizeof(int) * mark++]);
    printf("Magic Number: %d Images: %d Width: %d Height: %d\n", magic_number, *count, *width, *height);
    mark *= sizeof(int);
    return &buffer[mark];
}

uint8_t* idx1_header_read(uint8_t* buffer, int* count)
{
    int mark = 0;
    int magic_number = flip_int(&buffer[sizeof(int) * mark++]);
    *count = flip_int(&buffer[sizeof(int) * mark++]);
    printf("Magic Number: %d Labels: %d\n", magic_number, *count);
    mark *= sizeof(int);
    return &buffer[mark];
}

void idx3_to_png(const char* path, uint8_t* buffer, int width, int height)
{
    int i = 0;
    bmp_t bmp = bmp_new(width, height, 4);
    for (unsigned int y = 0; y < bmp.height; y++) {
        for (unsigned int x = 0; x < bmp.width; x++) {
            pixel_t p = unipixel(255 - buffer[i++]);
            memcpy(px_at(&bmp, x, y), &p, sizeof(pixel_t));
        }
    }
    bmp_write(path, &bmp);
    bmp_free(&bmp);
}

idx3_t idx3_load(char* path)
{ 
    idx3_t idx;
    idx.raw = file_read(path);
    idx.data = idx3_header_read(idx.raw, &idx.size, &idx.width, &idx.height);
    return idx;
}

idx1_t idx1_load(char* path)
{   
    idx1_t idx;
    idx.raw = file_read(path);
    idx.data = idx1_header_read(idx.raw, &idx.size);
    return idx;
}

void idx3_free(idx3_t idx)
{
    free(idx.raw);
}

void idx1_free(idx1_t idx)
{
    free(idx.raw);
}
