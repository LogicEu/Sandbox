#include <Nerv/Nerv.h>
#include <time.h>

int main(void)
{
    srand(time(NULL));
    nnetwork_t n = nnetwork_create(4, 2, 4, 4, 1);
    nnetwork_initialize(&n);

    int training_set_count = 4;
    float** inputs = (float**)malloc(sizeof(float*) * training_set_count);
    float** desired_outputs = (float**)malloc(sizeof(float*) * training_set_count);
    for (int i = 0; i < training_set_count; i++) {
        inputs[i] = (float*)malloc(sizeof(float) * n.layers[0].neuron_count);
        desired_outputs[i] = (float*)malloc(sizeof(float) * n.layers[n.layer_count - 1].neuron_count);
    }

    inputs[0][0] = 0.0f; inputs[0][1] = 0.0f;
    inputs[1][0] = 1.0f; inputs[1][1] = 0.0f;
    inputs[2][0] = 0.0f; inputs[2][1] = 1.0f;
    inputs[3][0] = 1.0f; inputs[3][1] = 1.0f;

    desired_outputs[0][0] = 0.0f;
    desired_outputs[1][0] = 1.0f;
    desired_outputs[2][0] = 1.0f;
    desired_outputs[3][0] = 0.0f;

    for (int i = 0; i < 10000; i++) {
        for (int j = 0; j < training_set_count; j++) {
            nnetwork_feed(&n, inputs[j]);
            forward_propagation(&n);
            //printf("Cost: %f\n", compute_cost(&n, desired_outputs[j]));
            backwards_propagation(&n, desired_outputs[j]);
            nnetwork_update(&n, 0.1f);
        }
    }

    printf("Trained network\n");
    printf("Enter two number as input to test output: ");
    layer_t* input_layer = n.layers;
    layer_t* output_layer = input_layer + n.layer_count - 1;
    neuron_t* n1 = input_layer->neurons;
    neuron_t* n2 = n1 + 1;
    scanf("%f %f", &n1->activation, &n2->activation);
    forward_propagation(&n);
    n1 = output_layer->neurons;
    printf("Output: %f\n", n1->activation);

    nnetwork_destroy(&n);
    return 0;
}
