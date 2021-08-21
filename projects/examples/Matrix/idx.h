#ifndef IDX_H
#define IDX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Imagine/Imagine.h>

typedef struct {
    int size, width, height;
    uint8_t* data, *raw;
} idx3_t;

typedef struct {
    int size;
    uint8_t* data, *raw;
} idx1_t;

int flip_int(uint8_t* data);
uint8_t* file_read(char* path);
uint8_t* idx3_header_read(uint8_t* buffer, int* count, int* width, int* height);
uint8_t* idx1_header_read(uint8_t* buffer, int* count);
void idx3_to_png(const char* path, uint8_t* buffer, int width, int height);

idx3_t idx3_load(char* path);
idx1_t idx1_load(char* path);
void idx3_free(idx3_t idx);
void idx1_free(idx1_t idx);

#ifdef __cplusplus
}
#endif
#endif