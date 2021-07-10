#include "../Nerv.h"

void nnetwork_save(const char* path, nnetwork_t* network)
{
    FILE* file = fopen(path, "wb");
    if (!file) {
        printf("Could not open file '%s' for writing\n", path);
        return;
    }

    fwrite(network, sizeof(nnetwork_t), 1, file);
    fwrite(network->layers, sizeof(layer_t), network->layer_count, file);
    for (int i = 0; i < network->layer_count; i++) {
        layer_t* layer = network->layers + i;
        fwrite(layer->neurons, sizeof(neuron_t), layer->neuron_count, file);

        if (i >= network->layer_count - 1) continue;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            fwrite(neuron->output_weights, sizeof(float), next_layer->neuron_count, file);
            fwrite(neuron->doutput_weights, sizeof(float), next_layer->neuron_count, file);
        }
    }
    printf("Succesfully saved Neural Network '%s'\n", path);
    fclose(file);
}

nnetwork_t nnetwork_load(const char* path)
{
    nnetwork_t network;
    network.layer_count = 0;
    network.layers = NULL;

    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return network;
    }

    fread(&network, sizeof(nnetwork_t), 1, file);
    printf("Network Layer Count: %d\n", network.layer_count);
    network.layers = (layer_t*)malloc(sizeof(layer_t) * network.layer_count);
    fread(network.layers, sizeof(layer_t), network.layer_count, file);
    for (int i = 0; i < network.layer_count; i++) {
        layer_t* layer = network.layers + i;
        layer->neurons = (neuron_t*)malloc(sizeof(neuron_t) * layer->neuron_count);
        fread(layer->neurons, sizeof(neuron_t), layer->neuron_count, file);
        printf("Layer Neuron Count: %d\n", layer->neuron_count);
        if (i >= network.layer_count - 1) continue;

        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            printf("Neuron Act: %f\n", neuron->activation);
            neuron->output_weights = (float*)malloc(sizeof(float) * next_layer->neuron_count);
            neuron->doutput_weights = (float*)malloc(sizeof(float) * next_layer->neuron_count);
            fread(neuron->output_weights, sizeof(float), next_layer->neuron_count, file);
            fread(neuron->doutput_weights, sizeof(float), next_layer->neuron_count, file);
        }
    }
    printf("Succesfully loaded Neural Network '%s'\n", path);
    fclose(file);
    return network;
}