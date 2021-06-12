#ifndef MODULAR_INTERNAL_H
#define MODULAR_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Modular/Modular.h>

/* ===============================
 * MODULAR ENTITY COMPONENT SYSTEM
 * -----------internal------------
 * ===============================
 * ******************************/

typedef struct ComponentHash {
    unsigned int data_size, data_used, data_bytes;
    unsigned int* keys, keys_size, keys_used;
    Entity* entities;
    char* data;
} ComponentHash;

typedef struct ModuleHash {
    ComponentHash* components;
    unsigned int used, size, entity_count;
    unsigned int* destroyed_keys, destroyed_size, destroyed_used;
} ModuleHash;

typedef struct ModularSystem {
    ModuleHash* modules;
    unsigned int used, size;
    Module activated;
} ModularSystem;

void module_push(ModuleHash* module);
ModuleHash* module_get(Module module);
ModuleHash* module_get_activated();
unsigned int module_count();
void module_free(ModuleHash* module);
void module_push_destroyed_key(unsigned int key);
unsigned int module_pop_destroyed_key();

ComponentHash* component_get_hash(Component component);
void component_free(ComponentHash* component);
void component_push(ComponentHash* component);

void archetype_system_init();
void archetype_system_destroy();

#ifdef __cplusplus
}
#endif
#endif