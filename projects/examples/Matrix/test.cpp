#include <Matrix/Matrix.h>
#include <time.h>

float timer_start()
{
    return (float)clock()/CLOCKS_PER_SEC;
}

float timer_end(float start)
{
    return timer_start() - start;
}

Mat mat_init(uint16_t rows, uint16_t columns)
{
    Mat ret = matrix(rows, columns), *m; m = &ret;
    for (int y = 0; y < ret.rows; y++) {
        for (int x = 0; x < ret.columns; x++) {
            *MATRIX_AT(m, x, y) = x + y;
            //*MATRIX_AT(m, x, y) = (float)(x + y) / (float)(m->width + m->height);
        }
    }
    return ret;
}

int main(void)
{
    /*Mat A = matrix_scan();
    Mat B = matrix_scan();
    Mat C = matmul(&A, &B);
    matrix_print(&C);*/

    /*Mat m = mat_init(1000, 1000);
    Mat n = mat_init(1000, 1000);
    float t = timer_start();
    Mat r = matmul(&m, &n);
    printf("Time: %f\n", timer_end(t));
    */

    Mat m = matrix_scan();
    Vec v = vector_scan();
    Mat mv = vector_to_matrix(&v);
    Vec r = vector_by_matrix(&m, &v);
    Mat rr = matmul(&m, &mv);
    vector_print(&r);
    matrix_print(&rr);
    return 0;
}