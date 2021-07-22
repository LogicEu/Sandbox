#include "../Matrix.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

// z = w * a + b
// a = f(z)
// c = (a - y)^2
// d = 2(a - y) * f'(z)
// dw = a * d * g
// db = d * g

NeuralMatrix neural_matrix_create(int layer_count, ...)
{   
    int neuron_counts[layer_count];
    va_list args;
    va_start(args, layer_count);
    for (int i = 0; i < layer_count; i++) {
        neuron_counts[i] = va_arg(args, int);
    }
    va_end(args);

    NeuralMatrix nm;
    nm.layer_count = layer_count;
    nm.layers = (Layer*)malloc(sizeof(Layer) * layer_count);
    
    Layer* layer = nm.layers;
    for (int i = 0; i < layer_count; i++) {
        int next = 0;
        if (i < layer_count - 1) next = neuron_counts[i + 1];
        *(layer++) = layer_create(neuron_counts[i], next);
    } 

    return nm;
}

void neural_matrix_destroy(NeuralMatrix* nm)
{
    Layer* layer = nm->layers;
    for (int i = 0; i < nm->layer_count - 1; i++) {
        layer_destroy(layer);
        layer++;
    }
    layer_free_vectors(layer);
}

void neural_matrix_init(NeuralMatrix* nm)
{
    Layer* layer = nm->layers;
    for (int i = 0; i < nm->layer_count - 1; i++) {
        Mat* m = &layer->w; 
        float* f = m->data;
        for (int j = 0; j < m->columns * m->rows; j++) {
            *(f++) = rand_normal() / (float)layer->a.size;
        }
        layer++;
    }
}

static void vector_swap(Vec func, Vec* v)
{
    vector_copy(v, &func);
    vector_free(&func);
}

static Vec vector_by_matrix_plus(Mat* m, Vec* a, Vec* b)
{
    Vec ret = vector_by_matrix(m, a);
    vector_add(&ret, b);
    return ret;
}

void neural_matrix_propagate_forward(NeuralMatrix* nm)
{
    Layer* layer = nm->layers, *next_layer; next_layer = layer + 1;
    for (int i = 0; i < nm->layer_count - 1; i++) {
        // z = w * a + b;
        vector_swap(vector_by_matrix_plus(&layer->w, &layer->a, &next_layer->b), &next_layer->z);
        //vector_print(&next_layer->z);
        // a = f(x) 
        if (i < nm->layer_count - 2) vector_swap(vector_sigmoid(&next_layer->z), &next_layer->a);
        else vector_swap(vector_sigmoid(&next_layer->z), &next_layer->a);
        //vector_print(&next_layer->a);

        layer++;
        next_layer++;
    }
}

void neural_matrix_propagate_backwards(NeuralMatrix* nm, Vec* desired_output)
{
    Layer* layer = nm->layers + nm->layer_count - 1;
    
    Vec c = veccpy(&layer->a);
    vector_sub(&c, desired_output);
    vector_scale(&c, 2.0f);

    vector_swap(vector_dsigmoid(&layer->z), &layer->d);
    vector_hadamard(&layer->d, &c);
    //vector_print(&layer->d);

    vector_free(&c);
    
    Layer* next_layer = layer--;
    for (Layer* end = nm->layers - 1; layer != end; layer--) {
        c = vector_by_matrix_transposed(&layer->w, &next_layer->d);
        //vector_print(&dw);
        
        vector_swap(vector_dsigmoid(&layer->z), &layer->d);
        //vector_print(&dz);

        vector_hadamard(&layer->d, &c);
        //vector_print(&layer->d);

        vector_free(&c);

        next_layer--;
    }
}

static void matrix_minus_vec_by_vec(Mat* m, Vec* a, Vec* b)
{
    if (a->size != m->columns || b->size != m->rows) {
        printf("WARNING!\n");
    }
    for (int y = 0; y < m->rows; y++) {
        for (int x = 0; x < m->columns; x++) {
            *MATRIX_AT(m, x, y) -= (*VECTOR_AT(a, x)) * (*VECTOR_AT(b, y));
        }
    }
}

float neural_matrix_compute_cost(NeuralMatrix* nm, Vec* desired_output)
{
    float cost = 0.0f;
    Layer* layer = nm->layers + nm->layer_count - 1;
    Vec* a = &layer->a;
    for (int i = 0; i < a->size; i++) {
        float f = (*VECTOR_AT(a, i)) - (*VECTOR_AT(desired_output, i));
        cost += f * f;
    }
    return cost / (float)a->size;
}

void neural_matrix_update(NeuralMatrix* nm, float alpha)
{
    Layer* layer = nm->layers, *next_layer; next_layer = layer + 1;
    for (Layer* end = layer + nm->layer_count - 1; layer != end; layer++) {
        vector_scale(&next_layer->d, alpha);
        vector_sub(&next_layer->b, &next_layer->d);
        matrix_minus_vec_by_vec(&layer->w, &layer->a, &next_layer->d);
        next_layer++;
    }
}

/* ******* PRINT ******* */

void neural_matrix_print(NeuralMatrix* nm)
{
    printf("Network Matrix:\n");
    Layer* layer = nm->layers;
    for (Layer* end = layer + nm->layer_count; layer != end; layer++) {
        vector_print(&layer->a);
    }
}

void neural_matrix_print_input(NeuralMatrix* nm)
{
    printf("Network Input:\n");
    vector_print(&nm->layers->a);
}

void neural_matrix_print_output(NeuralMatrix* nm)
{
    printf("Network Output:\n");
    vector_print(&nm->layers[nm->layer_count - 1].a);
}

void neural_matrix_print_struct(NeuralMatrix* nm)
{
    printf("Network Matrix Structure:\nLayers: %d\n", nm->layer_count);
    Layer* layer = nm->layers;
    int i = 0;
    for (Layer* end = layer + nm->layer_count; layer != end; layer++) {
        printf("Layer %d - Neurons: %d\n", ++i, layer->a.size);
    }
}