#ifndef SPAWN_SERIALIZE_H
#define SPAWN_SERIALIZE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * ====================
 * SPAWN SERIALIZATION
 * ===================
 * */

#include <Omega.h>
#include <stdlib.h>
#include <stdarg.h>

void* buffer_load_bin(const char* path, size_t *size);
void buffer_read_bin(const char* path, void* buffer, size_t* size);
void buffer_write_bin(const char* path, void* buffer, size_t size);

void* buffer_load_fmt(const char* path, size_t *size, const char* fmt, unsigned int count, ...);
void buffer_read_fmt(const char* path, void* buffer, size_t* size, const char* fmt, unsigned int count, ...);
void buffer_write_fmt(const char* path, void* buffer, size_t size, const char* fmt, unsigned int count, ...);

#ifdef __cplusplus
}
#endif
#endif
