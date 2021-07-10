#include "../Nerv.h"
#include <stdlib.h>
#include <string.h>

void neuron_destroy(neuron_t* neuron)
{
    free(neuron->output_weights);
    free(neuron->doutput_weights);
}

neuron_t neuron_create(int out_weights_count)
{
    neuron_t neuron;
    memset(&neuron, 0, sizeof(neuron_t));
    neuron.output_weights = (float*)malloc(sizeof(float) * out_weights_count);
    neuron.doutput_weights = (float*)malloc(sizeof(float) * out_weights_count);
    memset(neuron.doutput_weights, 0, sizeof(float) * out_weights_count);
    return neuron;
}
