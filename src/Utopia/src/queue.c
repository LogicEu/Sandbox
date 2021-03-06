#include <Utopia/Utopia.h>

queue_t queue(unsigned int size, unsigned int bytes)
{
    queue_t queue;
    queue.size = size;
    queue.bytes = bytes;
    queue.used = queue.front = 0;
    queue.rear = size - 1;
    queue.data = malloc(size * bytes);
    return queue;
}

queue_t* queue_new(unsigned int size, unsigned int bytes)
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->size = size;
    queue->bytes = bytes;
    queue->used = queue->front = 0;
    queue->rear = size - 1;
    queue->data = malloc(size * bytes);
    return queue;
}

unsigned int queue_is_empty(queue_t* queue)
{
    return queue->used == 0;
}

unsigned int queue_is_full(queue_t* queue)
{
    return queue->used == queue->size;
}

void* queue_index(queue_t* queue, unsigned int index)
{
    return (void*)((char*)queue->data + index * queue->bytes);
}

void queue_resize(queue_t* queue, unsigned int new_size)
{
    if (new_size < queue->used) return;
    queue->size = new_size;
    queue->data = realloc(queue->data, queue->size * queue->bytes);
}

void queue_cut(queue_t* queue)
{
    queue->size = queue->used;
    queue->data = realloc(queue->data, queue->size * queue->bytes);
}

void queue_push(queue_t* queue, void* data)
{
    if (queue->data == NULL) queue->data = malloc(queue->size * queue->used);
    if (queue_is_full(queue)) queue_resize(queue, queue->size * 2);
    queue->rear = (queue->rear + 1) % queue->size;
    memcpy(queue_index(queue, queue->rear), data, queue->bytes);
    queue->used++;
}

void* queue_pop(queue_t* queue)
{
    if (queue_is_empty(queue)) return NULL;
    void* ptr = queue_index(queue, queue->front);
    queue->front = (queue->front + 1) % queue->size;
    queue->used--;
    return ptr;
}

void* queue_peek(queue_t* queue)
{
    if (queue_is_empty(queue)) return NULL;
    return queue_index(queue, queue->front);
}

void queue_free(queue_t* queue)
{
    if (queue->data == NULL) return;
    free(queue->data);
    queue->data = NULL;
    queue->used = 0;
}

void queue_destroy(queue_t* queue)
{
    if (queue == NULL) return;
    queue_free(queue);
    free(queue);
}

