#include <Matrix/Matrix.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef type 
#define type float
#endif

typedef struct {
    int rank;
    int* shape;
    type* data;
} Tensor;

Tensor tensor(int rank, ...)
{
    Tensor t;
    t.rank = rank;
    t.shape = (int*)malloc(sizeof(int) * rank);
    size_t data_size = 1;

    va_list args;
    va_start(args, rank);
    for (int i = 0; i < rank; i++) {
        t.shape[i] = va_arg(args, int);
        data_size *= t.shape[i];
    }
    va_end(args);

    t.data = (type*)calloc(data_size, sizeof(type));
    return t;
}

type* tensor_at(Tensor* t, ...)
{
    type* n = t->data;
    size_t size = 1;

    va_list args;
    va_start(args, t->rank);
    for (int i = 0; i < t->rank; i++) {
        n += size * va_arg(args, int);
        size *= t->shape[i];
    }
    va_end(args);

    return n;
}

Tensor tensor_free(Tensor* t)
{
    free(t->shape);
    free(t->data);
}