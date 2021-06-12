#ifndef Z_BUG_H
#define Z_BUG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

extern unsigned int self;

void zbug_init(const char* path, unsigned int self_check);
void zbug_exit();
void zbug_log(const char* format, ...);
void zbug_log_fmt(const char* _file_, unsigned int _line_, const char* _func_, const char* format, ...);
void zbug_mark(const char* _file_, unsigned int _line_, const char* _func_);

void* zbug_malloc(const char* _file_, unsigned int _line_, const char* func, size_t size);
void* zbug_realloc(const char* _file_, unsigned int _line_, const char* func, void* ptr, size_t size);
void zbug_free(const char* _file_, unsigned int _line_, const char* func, void* ptr);

void zbug_manager_init();
void zbug_manager_push(void* ptr, size_t size);
void zbug_manager_realloc(void* ptr, size_t size, void* new_ptr);
void zbug_manager_free(void* ptr);
void zbug_manager_exit();
void zbug_diagnostic();
size_t zbug_allocated();
size_t zbug_index();

void zbug_smart_init(const char* path, unsigned int self_check);
void zbug_smart_exit();
void* zbug_smart_malloc(const char* _file_, unsigned int _line_, const char* func, size_t size);
void* zbug_smart_realloc(const char* _file_, unsigned int _line_, const char* func, void* ptr, size_t size);
void zbug_smart_free(const char* _file_, unsigned int _line_, const char* func, void* ptr);

#define Z_LOG(fmt, ...) zbug_log(__FILE__, __LINE__, __func__, fmt, __VA_ARGS__)
#define Z_MARK zbug_mark(__FILE__, __LINE__, __func__)
#define Z_BUG printf("ZBug Mark: %u | %s | %s()\n", __LINE__, __FILE__, __func__)

#ifdef ZBUG_SMART
    #define ZBUG_DUMP
    #define zbug_init(path, self) zbug_smart_init(path, self)
    #define zbug_exit() zbug_smart_exit()
    #define zbug_malloc(_file_, _line_, func, size) zbug_smart_malloc(_file_, _line_, func, size)
    #define zbug_realloc(_file_, _line_, func, ptr, size) zbug_smart_realloc(_file_, _line_, func, ptr,  size)
    #define zbug_free(_file_, _line_, func, ptr) zbug_smart_free(_file_, _line_, func, ptr)
#endif

#ifdef ZBUG_DUMP
    #define malloc(size) zbug_malloc(__FILE__, __LINE__, __func__, size)
    #define realloc(ptr, size) zbug_realloc(__FILE__, __LINE__, __func__, ptr, size)
    #define free(ptr) zbug_free(__FILE__, __LINE__, __func__, ptr)
#endif

#ifdef __cplusplus
}
#endif
#endif