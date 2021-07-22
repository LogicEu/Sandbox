#include "../Matrix.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vec vector(int size)
{
    Vec v;
    v.size = size;
    size *= sizeof(float);
    v.data = (float*)malloc(size);
    memset(v.data, 0, size);
    return v;
}

Vec vector_create(int size, ...)
{
    Vec v = vector(size);
    float *f = v.data;
    va_list args;
    va_start(args, size);
    for (float* end = f + size; f != end; f++) {
        float n = va_arg(args, double);
        *f = n;
    }
    va_end(args);
    return v;
}

Vec univector(int size, float val)
{
    Vec v = vector(size);
    float* f = v.data;
    for (float* end = f + v.size; f != end; f++) {
        *f = val;
    }
    return v;
}

void vector_free(Vec* vec)
{
    free(vec->data);
}

Vec veccpy(Vec* vec)
{
    Vec v = vector(vec->size);
    memcpy(v.data, vec->data, v.size * sizeof(float));
    return v;
}

void vector_copy(Vec* dst, Vec* src)
{
    if (dst->size != src->size) {
        printf("Warning! vector_copy(destination != source) size\n");
        return;
    }
    memcpy(dst->data, src->data, sizeof(float) * dst->size);
}

void vector_add(Vec* dst, Vec* src)
{
    if (dst->size != src->size) {
        printf("Vector A (%d) and B (%d) are not the same size\n", dst->size, src->size);
        return;
    }

    float* f = dst->data, *n = src->data;
    for (float* end = f + dst->size; f != end; f++) {
        *f += *(n++);
    }
}

void vector_sub(Vec* dst, Vec* src)
{
    if (dst->size != src->size) {
        printf("Vector A (%d) and B (%d) are not the same size\n", dst->size, src->size);
        return;
    }
    
    float* f = dst->data, *n = src->data;
    for (float* end = f + dst->size; f != end; f++) {
        *f -= *(n++);
    }
}

void vector_hadamard(Vec* dst, Vec* src)
{
    if (dst->size != src->size) {
        printf("Hadamard Vector Product: vectors are not the same size\n");
        return;
    }

    float* f = dst->data, *n = src->data;
    for (float* end = f + dst->size; f != end; f++) {
        *f *= *(n++);
    }
}

void vector_scale(Vec* v, float n)
{
    float *f = v->data;
    for (float* end = f + v->size; f != end; f++) {
        *f *= n;
    }
}

Vec vector_by_matrix(Mat* mat, Vec* vec)
{
    Vec ret = vector(mat->rows);
    if (vec->size != mat->columns) {
        printf("Vector size must be equal to matrix columns\n");
        return ret;
    }

    float* f = ret.data;
    for (int y = 0; y < mat->rows; y++) {
        for (int x = 0; x < mat->columns; x++) {
            *f += (*VECTOR_AT(vec, x)) * (*MATRIX_AT(mat, x, y));
        }
        f++;
    }

    return ret;
}

Vec vector_by_matrix_transposed(Mat* mat, Vec* vec)
{
    Mat m;
    memcpy(&m, mat, sizeof(Mat));
    int col = m.columns;
    m.columns = m.rows;
    m.rows = col;
    return vector_by_matrix(&m, vec);
}

Mat vector_to_matrix(Vec* v)
{
    Mat m = matrix(v->size, 1);
    memcpy(m.data, v->data, sizeof(float) * v->size);
    return m;
}

void vector_print(Vec* vec)
{
    printf("VECTOR - Size: %d\n", vec->size);
    float* f = vec->data;
    for (float* end = f + vec->size; f != end; f++) {
        printf("[ %f ]\n", *f);
    }
}

Vec vector_scan()
{
    int size;
    printf("Enter size of vector: ");
    scanf("%d", &size);
    Vec v = vector(size);
    for (int i = 0; i < size; i++) {
        printf("%d: ", i + 1);
        scanf("%f", v.data + i);
    }
    vector_print(&v);
    return v;
}

Vec vector_relu(Vec* v)
{
    Vec ret = vector(v->size);
    float* r = ret.data, *n = v->data;
    for (float* end = r + ret.size; r != end; r++) {
        *r = relu(*(n++));
    }
    return ret;
}

Vec vector_drelu(Vec* v)
{
    Vec ret = vector(v->size);
    float* r = ret.data, *n = v->data;
    for (float* end = r + ret.size; r != end; r++) {
        *r = drelu(*(n++));
    }
    return ret;
}

Vec vector_sigmoid(Vec* v)
{
    Vec ret = vector(v->size);
    float* r = ret.data, *n = v->data;
    for (float* end = r + ret.size; r != end; r++) {
        *r = sigmoid(*(n++));
    }
    return ret;
}

Vec vector_dsigmoid(Vec* v)
{
    Vec ret = vector(v->size);
    float* r = ret.data, *n = v->data;
    for (float* end = r + ret.size; r != end; r++) {
        *r = dsigmoid(*(n++));
    }
    return ret;
}

Vec vector_sigderiv(Vec* v)
{
    Vec ret = vector(v->size);
    float* r = ret.data, *n = v->data;
    for (float* end = r + ret.size; r != end; r++) {
        *r = sigderiv(*(n++));
    }
    return ret;
}

Vec vector_softmax(Vec* v)
{
    float* n = v->data, total = 0.0f, *f;
    for (float* end = n + v->size; n != end; n++) {
        total += *n;
    }

    Vec ret = vector(v->size);
    n = ret.data, f = v->data;
    for (float* end = n + ret.size; n != end; n++) {
        *n = *(f++) / total;
    }
    return ret;
}