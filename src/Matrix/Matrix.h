#ifndef MATRIX_MATH_H
#define MATRIX_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Omega.h>
#include <stdint.h>

/*******************************
 * MATRIX MATH & LINEAR ALG. LIB
 * -----------------------------
 *                      @eulogic
 ******************************/

typedef struct Vec {
    int size;
    float* data;
} Vec;

typedef struct Mat {
    uint16_t rows, columns;
    float* data;
} Mat;

typedef struct {
    Mat w;
    Vec b, z, a, d;
} Layer;

typedef struct {
    int layer_count;
    Layer* layers;
} NeuralMatrix;

#define MATRIX_AT(m, x, y) (m->data + (m->columns * y) + x)
#define VECTOR_AT(v, x) (v->data + x)

float normalize(float x);
uint8_t ftou8(float normalized);
float u8tof(uint8_t u);
float sigmoid(float x);
float sigderiv(float sig);
float dsigmoid(float x);
float relu(float x);
float drelu(float x);
float rand_normal();

Vec vector(int size);
Vec vector_create(int size, ...);
Vec univector(int size, float val);
void vector_free(Vec* vector);
Vec veccpy(Vec* vec);
void vector_print(Vec* vec);
Vec vector_by_matrix(Mat* mat, Vec* vec);
Vec vector_by_matrix_transposed(Mat* mat, Vec* vec);
Vec vector_scan();
void vector_add(Vec* dst, Vec* src);
void vector_sub(Vec* dst, Vec* src);
void vector_hadamard(Vec* dst, Vec* src);
Vec vector_sigmoid(Vec* v);
Vec vector_dsigmoid(Vec* v);
Vec vector_sigderiv(Vec* v);
Vec vector_relu(Vec* v);
Vec vector_drelu(Vec* v);
Vec vector_softmax(Vec* v);
void vector_scale(Vec* v, float n);
void vector_copy(Vec* dst, Vec* src);

Mat matrix(uint16_t rows, uint16_t columns);
Mat matrix_identity(int size);
void matrix_free(Mat* mat);
Mat matcpy(Mat* mat);
Mat matrix_scale(Mat* mat, float scale);
Mat matmul(Mat* a, Mat* b);
void matrix_print(Mat* mat);
Mat matrix_scan();
Mat matrix_hadamard(Mat* a, Mat* b);
Mat matrix_transpose(Mat* m);

Mat vector_to_matrix(Vec* v);

Layer layer_create(int layer_size, int next_layer_size);
void layer_destroy(Layer* layer);
void layer_free_vectors(Layer* layer);
void layer_free_matrix(Layer* layer);

NeuralMatrix neural_matrix_create(int layer_count, ...);
void neural_matrix_destroy(NeuralMatrix* nm);
void neural_matrix_init(NeuralMatrix* nm);
void neural_matrix_propagate_forward(NeuralMatrix* nm);
void neural_matrix_propagate_backwards(NeuralMatrix* nm, Vec* desired_output);
void neural_matrix_update(NeuralMatrix* nm, float alpha);
float neural_matrix_compute_cost(NeuralMatrix* nm, Vec* desired_output);

void neural_matrix_print(NeuralMatrix* nm);
void neural_matrix_print_input(NeuralMatrix* nm);
void neural_matrix_print_output(NeuralMatrix* nm);
void neural_matrix_print_struct(NeuralMatrix* nm);

#ifdef __cplusplus
}
#endif
#endif
