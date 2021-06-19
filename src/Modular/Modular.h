#ifndef MODULAR_ECS_H
#define MODULAR_ECS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ===============================
 * MODULAR ENTITY COMPONENT SYSTEM
 * ===============================
 * ******************************/

#include <Omega.h>

#ifndef MODULAR_RESERVE
    #define MODULAR_RESERVE 32
#endif

typedef unsigned int Archetype;
typedef unsigned int Entity;
typedef unsigned int Component;
typedef unsigned int Module;

void modularInit();
void modularExit();

Module module_create(unsigned int count, ...);
void module_destroy(Module module);
void module_activate(Module module);
Module module_current();

void module_save(const char* path, Module module);
Module module_load(const char* path);

Component component_create(unsigned int bytes);
void component_destroy(Component component);
unsigned int component_count();
void* component_get(Component component);
unsigned int component_get_size(Component component);
unsigned int component_entity_count(Component component);

Archetype archetype_create(unsigned int count, ...);
void archetype_destroy(Archetype archetype);
void archetype_add_component(Archetype archetype, Component component);
void archetype_remove_component(Archetype archetype, Component component);
Entity archetype_entity(Archetype archetype, unsigned int count, ...);

Entity entity_new();
Entity entity_create(unsigned int component_count, ...);
unsigned int entity_count();
void entity_add_component(Entity entity, Component component, void* data);
void entity_remove_component(Entity entity, Component component);
unsigned int entity_has(Entity entity, Component component);
void* entity_get(Entity entity, Component component);
void entity_set(Entity entity, Component component, void* data);
void entity_destroy(Entity entity);
Entity entity_find(Component component, unsigned int component_index);

#ifdef __cplusplus
}
#endif
#endif
