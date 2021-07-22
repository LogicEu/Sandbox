#ifndef NERV_NN_H
#define NERV_NN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Omega.h>
#include <stdint.h>

/*******************************
 * NERV NEURAL NETWORK C LINRARY
 * -----------------------------
 *                      @eulogic
 ******************************/

typedef struct neuron_t {
    float activation, dactivation;
    float *output_weights, *doutput_weights;
    float bias, dbias;
    float z, dz;
} neuron_t;

typedef struct layer_t {
    int neuron_count;
    neuron_t* neurons;
} layer_t;

typedef struct nnetwork_t {
    int layer_count;
    layer_t* layers;
} nnetwork_t;

neuron_t neuron_create(int out_weights_count);
void neuron_destroy(neuron_t* neuron);

layer_t layer_create(int neuron_count);
void layer_destroy(layer_t* layer);

nnetwork_t nnetwork_create(int layer_count, ...);
void nnetwork_destroy(nnetwork_t* network);
void nnetwork_initialize(nnetwork_t* network);
void nnetwork_update(nnetwork_t* network, float alpha);
void nnetwork_feed(nnetwork_t* network, float* input);
void nnetwork_train(nnetwork_t* network, float** inputs, float** desired_outputs, int iterations, int input_count, float learning_rate);

void nnetwork_print(nnetwork_t* network);
void nnetwork_print_input(nnetwork_t* network);
void nnetwork_print_output(nnetwork_t* network);

nnetwork_t nnetwork_load(const char* path);
void nnetwork_save(const char* path, nnetwork_t* network);

void backwards_propagation(nnetwork_t* network, float* desired_output);
void forward_propagation(nnetwork_t* network);
float compute_cost(nnetwork_t* network, float* desired_outpus);

float utof(uint8_t u);
float sigmoid(float x);
float dsigmoid(float x);
float relu(float x);
float drelu(float x);
float rand_weight();

#ifdef __cplusplus
}
#endif
#endif
