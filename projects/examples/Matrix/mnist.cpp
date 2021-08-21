#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <Matrix/Matrix.h>
#include "idx.h"

#define FILE_TRAIN_IMAGES "assets/MNIST/train-images-idx3-ubyte"
#define FILE_TRAIN_LABELS "assets/MNIST/train-labels-idx1-ubyte"
#define FILE_TEST_IMAGES "assets/MNIST/t10k-images-idx3-ubyte"
#define FILE_TEST_LABELS "assets/MNIST/t10k-labels-idx1-ubyte"

#define THREAD_COUNT 10

static pthread_t threads[THREAD_COUNT];
static NeuralMatrix main_model;
static NeuralMatrix models[THREAD_COUNT];
static Vec desired_outputs[THREAD_COUNT];
static idx3_t images;
static idx1_t labels;
static int iterations;
static float learning_rate;
const int output_size = 10;
static float thread_costs[THREAD_COUNT];

typedef struct {
    int x, y;
} i2;

float timer_start()
{
    return (float)clock()/CLOCKS_PER_SEC;
}

float timer_end(float start)
{
    return timer_start() - start;
}

void mnist_network_feed(NeuralMatrix* network, uint8_t* image)
{
    Vec* v = &network->layers->a;
    float* f = v->data;
    int i = 0;
    for (float* end = f + v->size; f != end; f++) {
        *f = u8tof(image[i++]);
    }
}

void set_desired_outputs(uint8_t u, Vec* v)
{
    memset(v->data, 0, sizeof(float) * v->size);
    v->data[u] = 1.0f;
}

int check_output(NeuralMatrix* network)
{
    Layer* output_layer = network->layers + network->layer_count - 1;
    Vec* a = &output_layer->a;
    int index, i = 0;
    float* n = a->data, max = -1000.0f;
    for (float* end = n + a->size; n != end; n++) {
        if (*n > max) {
            max = *n;
            index = i;
        }
        i++;
    }
    return index;
}

void neural_matrix_train(NeuralMatrix* model)
{
    int image_size = images.width * images.height;
    float cost = 0.0f, t = timer_start();
    for (int i = 0; i < images.size; i++) {
        mnist_network_feed(model, &images.data[i * image_size]);
        set_desired_outputs(labels.data[i], desired_outputs);
        neural_matrix_propagate_forward(model);
        cost += neural_matrix_compute_cost(model, desired_outputs);
        neural_matrix_propagate_backwards(model, desired_outputs);
        neural_matrix_update(model, learning_rate);
    }
    printf("Cost %f - Time: %f\n", cost / images.size, timer_end(t));
}

static float sum_cost()
{
    float n = 0.0f;
    for (int i = 0; i < THREAD_COUNT; i++) {
        n += thread_costs[i];
    }
    return n;
}

static void model_zero(NeuralMatrix* model)
{
    Layer* l = model->layers;
    Layer* ll = l + 1;
    
    for (Layer* end = l + model->layer_count - 1; l != end; l++) {
        
        memset(l->w.data, 0, sizeof(float) * l->w.columns * l->w.rows);
        memset(ll->b.data, 0, sizeof(float) * ll->b.size);
        
        ll++;
    }
}

void sum_models()
{
    model_zero(&main_model);

    for (int i = 0; i < THREAD_COUNT; i++) {

        Layer* lay = main_model.layers, *l = models[i].layers;
        Layer* llay = lay + 1, *ll = l + 1;

        for (int j = 0; j < main_model.layer_count - 1; j++) {

            Vec* vb = &llay->b;
            float* n = ll->b.data;

            for (int k = 0; k < ll->b.size; k++) {
                *VECTOR_AT(vb, k) += *(n++) / (float)THREAD_COUNT;
            }

            Mat* w = &lay->w;
            n = l->w.data;

            for (int y = 0; y < l->w.rows; y++) {
                for (int x = 0; x < l->w.columns; x++) {
                    *MATRIX_AT(w, x, y) += *(n++) / (float)THREAD_COUNT;
                }
            }

            llay++;
            lay++;
            ll++;
            l++;
        }
    }

}

void update_model(NeuralMatrix* model)
{
    for (int i = 0; i < model->layer_count - 1; i++) {
        Layer* l = model->layers + i;
        Layer* ll = l + 1;
        memcpy(l->w.data, main_model.layers[i].w.data, sizeof(float) * l->w.rows * l->w.columns);
        memcpy(ll->b.data, main_model.layers[i + 1].b.data, sizeof(float) * ll->b.size);
    }
}

void* threads_forw(void* arg)
{
    i2 num = *(i2*)arg;
    if (num.y > 0) update_model(&models[num.x]);
    mnist_network_feed(&models[num.x], &images.data[(num.x + num.y) * images.width * images.height]);
    set_desired_outputs(labels.data[num.x + num.y], &desired_outputs[num.x]);
    neural_matrix_propagate_forward(&models[num.x]);
    neural_matrix_propagate_backwards(&models[num.x], &desired_outputs[num.x]);
    thread_costs[num.x] = neural_matrix_compute_cost(&models[num.x], &desired_outputs[num.x]);
    neural_matrix_propagate_backwards(&models[num.x], &desired_outputs[num.x]);
    neural_matrix_update(&models[num.x], learning_rate);
    return NULL;
}

void neural_matrix_train_parallel()
{
    float cost = 0.0f, time = timer_start();
    //float thread_time = 0.0f, cost_time = 0.0f, sum_time = 0.0f;
    for (int i = 0; i < images.size; i += THREAD_COUNT) {
        
        i2 args[THREAD_COUNT];
        
        //float t = timer_start();
        for (int j = 0; j < THREAD_COUNT; j++) {
            args[j].x = j;
            args[j].y = i;
            pthread_create(&threads[j], NULL, *threads_forw, &args[j]);
        }

        for (int j = 0; j < THREAD_COUNT; j++) {
            pthread_join(threads[j], NULL);
        }
        //thread_time += timer_end(t);

        //t = timer_start();
        cost += sum_cost();
        //cost_time += timer_end(t);

        //t = timer_start();
        sum_models();
        //sum_time += timer_end(t);
    }
    printf("Cost %f - Time: %f\n", cost / images.size, timer_end(time) / (float)THREAD_COUNT);
    //float f = (float)images.size / (float)THREAD_COUNT;
    //printf("Thread Time: %f Cost Time: %f Sum Time: %f\n", thread_time / f, cost_time / f, sum_time / f);
}

void* threads_init(void* arg)
{
    int num = *(int*)arg;
    desired_outputs[num] = vector(output_size);
    models[num] = neural_matrix_copy(&main_model);
    return NULL;
}

int main(int argc, char** argv)
{
    iterations = 10;
    learning_rate = 0.1f;

    printf("Matrix Neural Network@\n");
    srand(time(NULL));

    if (argc > 1) iterations = atoi(argv[1]);
    if (argc > 2) learning_rate = atof(argv[2]);

    /***** TRAIN DATA ******/

    images = idx3_load(FILE_TRAIN_IMAGES);
    labels = idx1_load(FILE_TRAIN_LABELS);
    int image_size = images.width * images.height;
    printf("Image Size: %dx%d - %d\n", images.width, images.height, image_size);

    /***** THREADS && NETWORK INITIALIZATION ******/

    main_model = neural_matrix_create(3, image_size, 128, output_size);
    neural_matrix_init(&main_model);
    neural_matrix_print_struct(&main_model);

    int thr_index[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; i++) {
        thr_index[i] = i;
        pthread_create(&threads[i], NULL, *threads_init, &thr_index[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    /***** NETWORK TRAINING ******/

    printf("Iterations: %d\n", iterations);
    printf("Learning Rate: %f\n", learning_rate);
    printf("Number of threads (batch size): %d\n", THREAD_COUNT);
    printf("Training network...\n");
    for (int i = 0; i < iterations; i++) {
        printf("Epoch %d ", i + 1);
        //neural_matrix_train_parallel();
        neural_matrix_train(&main_model);
        if (i == 4) learning_rate /= 10.0f;
    }
    
    idx3_free(images);
    idx1_free(labels);

    // **** TEST DATA *****

    images = idx3_load(FILE_TEST_IMAGES);
    labels = idx1_load(FILE_TEST_LABELS);
    image_size = images.width * images.height;
    printf("Image Size: %dx%d - %d\n", images.width, images.height, image_size);

    int med = 0;
    int imax, imin, omax, omin;
    float cost = 0.0f, min = 100.0f, max = -100.0f;
    for (int i = 0; i < images.size; i++) {
        mnist_network_feed(&main_model, &images.data[i * image_size]);
        neural_matrix_propagate_forward(&main_model);
        int out = check_output(&main_model);
        set_desired_outputs(labels.data[i], &desired_outputs[0]);
        float c = neural_matrix_compute_cost(&main_model, &desired_outputs[0]);
        if (c > max) {
            max = c;
            imax = i;
            omax = out;
        } if (c < min) {
            min = c;
            imin = i;
            omin = out;
        }
        cost += c;

        if (out == (int)(unsigned int)labels.data[i]) med++;
    }
    float accuracy = (float)med * 100.0f / (float)images.size;
    printf("Test Accuracy: %f\n", accuracy);
    printf("Cost: %f\n", cost / (float)images.size);
    printf("Min Cost: Expected %d Desired: %u\n", omin, labels.data[imin]);
    printf("Max Cost: Expected: %d Desired: %u\n", omax, labels.data[imax]);
    idx3_to_png("min.png", images.data + imin * image_size, images.width, images.height);
    idx3_to_png("max.png", images.data + imax * image_size, images.width, images.height);
    system("open min.png");
    system("open max.png");

    idx3_free(images);
    idx1_free(labels);
    neural_matrix_destroy(&main_model);
    return 0;
}

