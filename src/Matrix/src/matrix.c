#include <Matrix/Matrix.h>
#include <stdlib.h>
#include <string.h>

Mat matrix(uint16_t rows, uint16_t columns)
{
    Mat mat;
    mat.rows = rows;
    mat.columns = columns;
    int size = rows * columns * sizeof(float);
    mat.data = (float*)malloc(size);
    memset(mat.data, 0, size);
    return mat;
}

void matrix_free(Mat* mat)
{
    free(mat->data);
}

Mat matcpy(Mat* mat)
{
    Mat ret = matrix(mat->rows, mat->columns);
    memcpy(ret.data, mat->data, sizeof(float) * mat->rows * mat->columns);
    return ret;
}

Mat matrix_identity(int size)
{
    Mat ret = matrix(size, size), *m; m = &ret;
    for (int i = 0; i < size; i++) {
        *MATRIX_AT(m, i, i) = 1.0;
    }
    return ret;
}

Mat matrix_scale(Mat* mat, float scale)
{
    Mat ret = matcpy(mat);
    float* f = ret.data;
    for (float* end = f + ret.rows * ret.columns; f != end; f++) {
        *f *= scale;
    }
    return ret;
}

Mat matrix_hadamard(Mat* a, Mat* b)
{
    Mat ret = matrix(a->rows, a->columns);
    if ((a->rows != b->rows) || (a->columns != b->columns)) {
        printf("Matrixes are not equal for Hadamard Product\n");
        return ret;
    }

    float* f = ret.data, *af = a->data, *bf = a->data;
    for (int i = 0; i < ret.rows * ret.columns; i++) {
        *(f++) = (*(af++)) * (*(bf++));
    }
    return ret;
}

Mat matrix_transpose(Mat* m)
{
    Mat ret = matrix(m->columns, m->rows); Mat* r = &ret;
    for (int y = 0; y < m->rows; y++) {
        for (int x = 0; x < m->columns; x++) {
            *MATRIX_AT(r, y, x) = *MATRIX_AT(m, x, y);
        }
    }
    return ret;
}

Mat matmul(Mat* a, Mat* b)
{
    Mat ret = matrix(a->rows, b->columns);
    if (a->columns != b->rows) {
        printf("Number of columns in first matrix must be equal to number of rows in second\n");
        return ret;
    }

    float* f = ret.data;
    for (int y = 0; y < ret.rows; y++) {
        for (int x = 0; x < ret.columns; x++) {
            for (int z = 0; z < a->columns; z++) {
                *f += (*MATRIX_AT(a, z, y)) * (*MATRIX_AT(b, x, z));
            }
            f++;
        }
    }
    return ret;
}

void matrix_print(Mat* mat)
{
    printf("MATRIX\nRows: %u\tColumns: %u\n", mat->rows, mat->columns);
    for (int y = 0; y < (int)mat->rows; y++) {
        printf("[");
        for (int x = 0; x < (int)mat->columns; x++) {
            printf(" %f ", *MATRIX_AT(mat, x, y));
        }
        printf("]\n");
    }
    printf("\n");
}

Mat matrix_scan()
{
    uint16_t rows, columns;
    printf("Enter rows: ");
    scanf("%hu", &rows);
    printf("Enter columns: ");
    scanf("%hu", &columns);

    Mat mat = matrix(rows, columns);
    Mat* m = &mat;
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < columns; x++) {
            printf("%dx%d: ", y + 1, x + 1);
            scanf("%f", MATRIX_AT(m, x, y));
        }
    }
    matrix_print(m);
    return mat;
}
