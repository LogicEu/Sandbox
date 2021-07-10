#include "../Nerv.h"
#include <stdlib.h>
#include <stdarg.h>

#define PRINT_SEPARATOR "-----------------------------\n"

nnetwork_t nnetwork_create(int layer_count, ...)
{
    nnetwork_t network;
    network.layer_count = layer_count;
    network.layers = (layer_t*)malloc(sizeof(layer_t) * layer_count);
    
    va_list args;
    va_start(args, layer_count);
    for (int i = 0; i < layer_count; i++) {
        network.layers[i] = layer_create(va_arg(args, int));
    }
    va_end(args);

    for (int i = 0; i < network.layer_count - 1; i++) {
        layer_t* layer = network.layers + i;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            *neuron = neuron_create(next_layer->neuron_count);
        }
    }

    return network;
}

void nnetwork_destroy(nnetwork_t* network)
{
    layer_t* l = network->layers;
    for (layer_t* end = l + network->layer_count - 1; l != end; l++) {
        layer_destroy(l);
    }
    free(l->neurons);
    free(network->layers);
}

void nnetwork_initialize(nnetwork_t* network)
{
    for (int i = 0; i < network->layer_count - 1; i++) {
        layer_t* layer = network->layers + i;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            for (int k = 0; k < next_layer->neuron_count; k++) {
                neuron->output_weights[k] = rand_weight();
            }

            if (!i) continue;
            neuron->bias = rand_weight();
        }
    }

    layer_t* last_layer = network->layers + network->layer_count - 1;
    for (int i = 0; i < last_layer->neuron_count; i++) {
        neuron_t* neuron = last_layer->neurons + i;
        neuron->bias = 0.0f;
    }
}

void nnetwork_update(nnetwork_t* network, float alpha)
{
    for (int i = 0; i < network->layer_count - 1; i++) {
        layer_t* layer = network->layers + i;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            for (int k = 0; k < next_layer->neuron_count; k++) {
                neuron->output_weights[k] -= alpha * neuron->doutput_weights[k];
            }
            neuron->bias -= alpha * neuron->dbias;
        }
    }
}

void nnetwork_feed(nnetwork_t* network, float* input)
{
    layer_t* input_layer = network->layers;
    for (int i = 0; i < input_layer->neuron_count; i++) {
        neuron_t* neuron = input_layer->neurons + i;
        neuron->activation = input[i];
    }
}

void nnetwork_train(nnetwork_t* network, float** inputs, float** desired_outputs, int iterations, int input_count, float learning_rate)
{
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < input_count; j++) {
            nnetwork_feed(network, inputs[j]);
            forward_propagation(network);
            //compute_cost(j)
            backwards_propagation(network, desired_outputs[j]);
            nnetwork_update(network, learning_rate);
        }
    }
}

void nnetwork_print_output(nnetwork_t* network)
{
    printf(PRINT_SEPARATOR);
    printf("OUTPUT\n");
    layer_t* output_layer = network->layers + network->layer_count - 1;
    for (int i = 0; i < output_layer->neuron_count; i++) {
        neuron_t* neuron = output_layer->neurons + i;
        printf("%d: %f\n", i, neuron->activation);
    }
    printf(PRINT_SEPARATOR);
}

void nnetwork_print_input(nnetwork_t* network)
{
    printf(PRINT_SEPARATOR);
    printf("INPUT\n");
    layer_t* input_layer = network->layers;
    for (int i = 0; i < input_layer->neuron_count; i++) {
        neuron_t* neuron = input_layer->neurons + i;
        printf("%d: %f\n", i, neuron->activation);
    }
    printf(PRINT_SEPARATOR);
}

void nnetwork_print(nnetwork_t* network)
{
    printf(PRINT_SEPARATOR);
    printf("NETWORK\n");
    for (int i = 0; i < network->layer_count; i++) {
        layer_t* layer = network->layers + i;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            printf("Layer %d Neuron %d: %f %f\n", i + 1, j, neuron->activation, neuron->bias);
        }
    }
    printf(PRINT_SEPARATOR);
}