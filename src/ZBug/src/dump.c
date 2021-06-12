#include <ZBug/ZBug.h>

void* zbug_malloc(const char* _file_, unsigned int _line_, const char* func, size_t size)
{
    zbug_log("malloc(%zu) | %u / '%s' / '%s' | ", size, _line_, _file_, func);
    void* ret = malloc(size);
    zbug_log("(%p)\n", ret);
    return ret;
}

void* zbug_realloc(const char* _file_, unsigned int _line_, const char* func, void* ptr, size_t size)
{
    zbug_log("realloc(%p, %zu) | %u - '%s' - '%s' | ", ptr, size, _line_, _file_, func);
    void* ret = realloc(ptr, size);
    zbug_log("(%p)\n", ret);
    return ret;
}

void zbug_free(const char* _file_, unsigned int _line_, const char* func, void* ptr)
{
    zbug_log("free(%p) | %u - '%s' - '%s' | ", ptr, _line_, _file_, func);
    free(ptr);
    zbug_log("(%p)\n", ptr);
}