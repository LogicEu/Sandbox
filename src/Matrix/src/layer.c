#include <Matrix/Matrix.h>

Layer layer_create(int layer_size, int next_layer_size)
{
    Layer layer;
    layer.a = vector(layer_size);
    layer.b = vector(layer_size);
    layer.z = vector(layer_size);
    layer.d = vector(layer_size);
    if (!next_layer_size) return layer;
    layer.w = matrix(next_layer_size, layer_size);
    return layer;
}

void layer_free_matrix(Layer* layer)
{
    matrix_free(&layer->w);
}

void layer_free_vectors(Layer* layer)
{
    vector_free(&layer->a);
    vector_free(&layer->b);
    vector_free(&layer->z);
    vector_free(&layer->d);
}

void layer_destroy(Layer* layer)
{
    layer_free_matrix(layer);
    layer_free_vectors(layer);
}