#include "BNCommon.h"

entity_t* entity_find_by_id(list_t* list, unsigned int id)
{
    node_t* node = list->head;
    while (node != NULL) {
        entity_t* e = (entity_t*)node->data;
        if (id == e->id) return e;
        node = node->next;
    }
    return NULL;
}

entity_t* entity_find_by_id_ptr(list_t* list, unsigned int id)
{
    node_t* node = list->head;
    while (node != NULL) {
        entity_t* e = *(entity_t**)node->data;
        if (id == e->id) return e;
        node = node->next;
    }
    return NULL;
}

unsigned int entity_find_new_id(list_t* list)
{
    unsigned int ret = 0;
    while (entity_find_by_id(list, ret) != NULL) {
        ret++;
    }
    return ret;
}

void entity_remove_by_id(list_t* list, unsigned int id) 
{
    node_t* node = list->head, *next;
    while (node != NULL) {
        entity_t* p = (entity_t*)node->data;
        next = node->next;
        if (p->id == id) {
            list_remove_node(list, node);
            break;
        }
        node = next;
    }
}

void entity_remove_by_id_ptr(list_t* list, unsigned int id) 
{
    node_t* node = list->head, *next;
    while (node != NULL) {
        entity_t* p = *(entity_t**)node->data;
        next = node->next;
        if (p->id == id) {
            list_remove_node(list, node);
            break;
        }
        node = next;
    }
}