#include <Utopia/Utopia.h>
#include <stdio.h>
#include <time.h>

void benchmark_array(const unsigned int size)
{
    float acc = 0.0f;
    float tstart = (float)clock()/CLOCKS_PER_SEC;

    float array[size];
    for (int i = 0; i < size; i++) {
        array[i] = (float)i;
    }

    for (int i = 0; i < size; i++) {
        acc += array[i];
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("Array count is %f\n", acc);
    printf("Array benchmark is %f seconds\n", delta_time);
}

void benchmark_dynamic_array(const unsigned int size)
{
    float acc = 0.0f;
    array_t* array = array_new(size, sizeof(float));

    float tstart = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < size; i++) {
        float f = (float)i;
        array_push(array, &f);
    }

    float* ptr = (float*)array->data;
    for (float* end = ptr + array->used; ptr != end; ptr++) {
        acc += *ptr;
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("Dynamic array count is %f\n", acc);
    printf("Dynamic array benchmark is %f seconds\n", delta_time);

    array_destroy(array);
}

void benchmark_list(const unsigned int size)
{
    float acc = 0.0f;
    list_t* list = list_new(sizeof(float));

    float tstart = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < size; i++) {
        float f = (float)i;
        list_push(list, &f);
    }

    node_t* node = list->head;
    while (node != NULL) {
        acc += *(float*)node->data;
        node = node->next;
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("List count is %f\n", acc);
    printf("List benchmark is %f seconds\n", delta_time);

    list_destroy(list);
}

void benchmark_stack(const unsigned int size)
{
    float acc = 0.0f;
    ustack_t* stack = stack_new(size, sizeof(float));

    float tstart = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < size; i++) {
        float f = (float)i;
        stack_push(stack, &f);
    }

    while (!stack_is_empty(stack)) {
        acc += *(float*)stack_pop(stack);
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("Stack count is %f\n", acc);
    printf("Stack benchmark is %f seconds\n", delta_time);

    stack_destroy(stack);
}

void benchmark_queue(const unsigned int size)
{
    float acc = 0.0f;
    queue_t* queue = queue_new(size, sizeof(float));

    float tstart = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < size; i++) {
        float f = (float)i;
        queue_push(queue, &f);
    }

    while (!queue_is_empty(queue)) {
        acc += *(float*)queue_pop(queue);
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("Queue count is %f\n", acc);
    printf("Queue benchmark is %f seconds\n", delta_time);

    queue_destroy(queue);
}

/*void benchmark_vector(const unsigned int size)
{
    float acc = 0.0f;
    std::vector<float> v;

    float tstart = (float)clock()/CLOCKS_PER_SEC;
    for (int i = 0; i < size; i++) {
        v.push_back((float)i);
    }

    for (int i = 0; i < v.size(); i++) {
        acc += v[i];
    }
    
    float delta_time = (float)clock()/CLOCKS_PER_SEC - tstart;
    printf("Vector count is %f\n", acc);
    printf("Vector benchmark is %f seconds\n", delta_time);

    v.clear();
}*/

int main(int argc, char** argv)
{
    unsigned int size = 10000;
    if (argc > 1) size = atoi(argv[1]);

    benchmark_array(size);
    benchmark_dynamic_array(size);
    benchmark_list(size);
    benchmark_stack(size);
    benchmark_queue(size);
    //benchmark_vector(size);

    array_t* array = array_new(100, sizeof(float));
    for (int i = 0; i < array->size; i++) {
        float f = (float)i;
        array_push(array, &f);
    }

    for (int i = 0; i < 10; i++) {
        array_remove(array, 90 - i);
    }

    for (int i = 0; i < array->used; i++) {
        printf("%f\n", *(float*)array_index(array, i));
    }
    return 0;
}
