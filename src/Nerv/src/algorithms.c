#include "../Nerv.h"

void forward_propagation(nnetwork_t* network)
{
    for (int i = 1; i < network->layer_count; i++) {
        layer_t* layer = network->layers + i;
        layer_t* prev_layer = layer - 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            neuron->z = neuron->bias;

            for (int k = 0; k < prev_layer->neuron_count; k++) {
                neuron_t* prev_neuron = prev_layer->neurons + k;
                neuron->z += prev_neuron->output_weights[j] * prev_neuron->activation;
            }

            //Relu for hidden layers
            if (i < network->layer_count - 1) {
                neuron->activation = relu(neuron->z);
            } else { //Sigmoid for output layer
                neuron->activation = sigmoid(neuron->z);
            }
        }
    }
}

void backwards_propagation(nnetwork_t* network, float* desired_output)
{
    layer_t* output_layer = network->layers + network->layer_count - 1;
    layer_t* prev_layer = output_layer - 1;

    // Output layer
    for (int i = 0; i < output_layer->neuron_count; i++) {
        neuron_t* neuron = output_layer->neurons + i;
        //printf("%f %f %f %f\n", neuron->activation, desired_output[i], neuron->z, dsigmoid(neuron->z));
        neuron->dz = (neuron->activation - desired_output[i]) * (neuron->activation * (1.0f - neuron->activation));
        for (int j = 0; j < prev_layer->neuron_count; j++) {
            neuron_t* prev_neuron = prev_layer->neurons + j;
            prev_neuron->doutput_weights[i] = neuron->dz * prev_neuron->activation;
            prev_neuron->dactivation = prev_neuron->output_weights[i] * neuron->dz;
        }
        neuron->dbias = neuron->dz;
    }

    // Hidden layers
    for (int i = network->layer_count - 2; i > 0; i--) {
        layer_t* layer = network->layers + i;
        layer_t* prev_layer = layer - 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            neuron->dz = drelu(neuron->z) * neuron->dactivation;

            for (int k = 0; k < prev_layer->neuron_count; k++) {
                neuron_t* prev_neuron = prev_layer->neurons + k;
                prev_neuron->doutput_weights[j] = neuron->dz * prev_neuron->activation;
                
                if (i <= 1) continue;
                prev_neuron->dactivation = prev_neuron->output_weights[j] * neuron->dz;
            }

            neuron->dbias = neuron->dz;
        }
    }
}

float compute_cost(nnetwork_t* network, float* desired_outpus)
{
    float total_cost = 0.0f;
    layer_t* output_layer = network->layers + network->layer_count - 1;
    for (int i = 0; i < output_layer->neuron_count; i++) {
        neuron_t* neuron = output_layer->neurons + i;
        float cost = desired_outpus[i] - neuron->activation;
        total_cost += (cost * cost) * 0.5f;
    }
    return total_cost / output_layer->neuron_count;
}