#include "../Nerv.h"
#include <stdlib.h>

void layer_destroy(layer_t* layer)
{
    for (int i = 0; i < layer->neuron_count; i++) {
        neuron_destroy(layer->neurons + i);
    }
    free(layer->neurons);
}

layer_t layer_create(int neuron_count)
{
    layer_t layer;
    layer.neuron_count = neuron_count;
    layer.neurons = (neuron_t*)malloc(sizeof(neuron_t) * neuron_count);
    return layer;
}
