#include <ZBug/ZBug.h>
#include <string.h>

typedef struct ZBugPair {
    void* ptr;
    size_t size;
} ZBugPair;

typedef struct ZBugManager {
    size_t size, used, allocated;
    ZBugPair* pairs;
} ZBugManager;

static ZBugManager zbug_manager;

static ZBugPair* zbug_find_pair(void* ptr)
{
    ZBugPair* p = zbug_manager.pairs;
    for (ZBugPair* end = p + zbug_manager.used; p != end; p++) {
        if (p->ptr == ptr) return p;
    }
    return NULL;
}

void zbug_manager_init()
{
    zbug_manager.size = 32;
    zbug_manager.used = 0;
    zbug_manager.allocated = 0;
    zbug_manager.pairs = (ZBugPair*)malloc(sizeof(ZBugPair) * zbug_manager.size);
    if (self) {
        zbug_log("malloc(%zu) | %u / %s / %s | ", zbug_manager.size, __LINE__, __FILE__, __func__);
        zbug_manager_push(zbug_manager.pairs, zbug_manager.size);
        zbug_log("(%p) [%zu] %zu\n", zbug_manager.pairs, zbug_allocated(), zbug_index());
    }
}

void zbug_manager_push(void* ptr, size_t size)
{
    void* pairs = zbug_manager.pairs;
    size_t s = zbug_manager.size;
    zbug_manager.allocated += size;
    if (zbug_manager.used >= zbug_manager.size) {
        zbug_manager.size *= 2;
        zbug_manager.pairs = (ZBugPair*)realloc(zbug_manager.pairs, sizeof(ZBugPair) * zbug_manager.size);
    }
    ZBugPair p = {ptr, size};
    memcpy(&zbug_manager.pairs[zbug_manager.used++], &p, sizeof(ZBugPair));
    
    if (s != zbug_manager.size && self) {
        zbug_log("realloc(%p, %zu) | %u / %s / %s | ", ptr, size, __LINE__, __FILE__, __func__);
        zbug_manager_realloc(pairs, zbug_manager.size, zbug_manager.pairs);
        zbug_log("(%p) [%zu]\n", zbug_manager.pairs, zbug_allocated());
    }
}

void zbug_manager_realloc(void* ptr, size_t size, void* new_ptr)
{
    ZBugPair* p = zbug_find_pair(ptr);
    if (p != NULL) {
        zbug_manager.allocated += size - p->size;
        p->size = size;
        p->ptr = new_ptr;
    }
}

void zbug_manager_free(void* ptr)
{
    ZBugPair* p = zbug_find_pair(ptr);
    if (p != NULL) {
        zbug_manager.allocated -= p->size;
        p->size = 0;
        p->ptr = NULL;
    }
}

void zbug_manager_exit()
{
    if (self) {
        zbug_log("free(%p) | %u / %s / %s | ", zbug_manager.pairs, __LINE__, __FILE__, __func__);
        zbug_manager_free(zbug_manager.pairs);
        zbug_log("(%p) [%zu]\n", zbug_manager.pairs, zbug_allocated());
    }
    free(zbug_manager.pairs);
}

void zbug_diagnostic()
{
    zbug_log("\nZBUG DIAGNOSTICS\n");

    size_t i = 0;
    ZBugPair* p = zbug_manager.pairs;
    for (ZBugPair* end = p + zbug_manager.used; p != end; p++) {
        if (p->ptr != NULL) {
            zbug_log("Index: %zu - Size: %zu: - Ptr: %p\n", i, p->size, p->ptr);
        }
        i++;
    }
}

size_t zbug_allocated()
{
    return zbug_manager.allocated;
}

size_t zbug_index()
{
    return zbug_manager.used;
}