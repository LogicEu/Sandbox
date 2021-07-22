#include <stdio.h>
#include <stdlib.h>
#include <Matrix/Matrix.h>
#include <Imagine/Imagine.h>

#define FILE_TRAIN_IMAGES "assets/MNIST/train-images-idx3-ubyte"
#define FILE_TRAIN_LABELS "assets/MNIST/train-labels-idx1-ubyte"
#define FILE_TEST_IMAGES "assets/MNIST/t10k-images-idx3-ubyte"
#define FILE_TEST_LABELS "assets/MNIST/t10k-labels-idx1-ubyte"

float timer_start()
{
    return (float)clock()/CLOCKS_PER_SEC;
}

float timer_end(float start)
{
    return timer_start() - start;
}

int flip_int(uint8_t* data)
{
    uint8_t buff[sizeof(int)];
    for (int i = 0; i < sizeof(int); i++) {
        buff[i] = data[sizeof(int) - 1 - i];
    }
    return *(int*)&buff[0];
}

uint8_t* file_read(char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t* buffer = (uint8_t*)malloc(size);
    fread(buffer, size, 1, file);
    fclose(file);
    return buffer;
}

uint8_t* idx3_header_read(uint8_t* buffer, int* count, int* width, int* height)
{
    int mark = 0;
    int magic_number = flip_int(&buffer[sizeof(int) * mark++]);
    *count = flip_int(&buffer[sizeof(int) * mark++]);
    *width = flip_int(&buffer[sizeof(int) * mark++]);
    *height = flip_int(&buffer[sizeof(int) * mark++]);
    printf("Magic Number: %d Images: %d Width: %d Height: %d\n", magic_number, *count, *width, *height);
    mark *= sizeof(int);
    return &buffer[mark];
}

uint8_t* idx1_header_read(uint8_t* buffer, int* count)
{
    int mark = 0;
    int magic_number = flip_int(&buffer[sizeof(int) * mark++]);
    *count = flip_int(&buffer[sizeof(int) * mark++]);
    printf("Magic Number: %d Labels: %d\n", magic_number, *count);
    mark *= sizeof(int);
    return &buffer[mark];
}

void idx3_to_png(const char* path, uint8_t* buffer, int width, int height)
{
    int i = 0;
    bmp_t bmp = bmp_new(width, height, 4);
    for (unsigned int y = 0; y < bmp.height; y++) {
        for (unsigned int x = 0; x < bmp.width; x++) {
            pixel_t p = unipixel(255 - buffer[i++]);
            memcpy(px_at(&bmp, x, y), &p, sizeof(pixel_t));
        }
    }
    bmp_write(path, &bmp);
    bmp_free(&bmp);
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

int main(int argc, char** argv)
{
    int iterations = 10;
    float learning_rate = 0.1f;
    Vec desired_outputs = vector(10);
    if (argc > 1) iterations = atoi(argv[1]);

    srand(time(NULL));
    printf("Matrix Neural Network@\n");

    /***** TRAIN DATA ******/

    uint8_t* train_images_raw = file_read(FILE_TRAIN_IMAGES);
    uint8_t* train_labels_raw = file_read(FILE_TRAIN_LABELS);
    
    int train_images_count, train_images_width, train_images_height, train_labels_count;
    uint8_t* train_images_data = idx3_header_read(train_images_raw, &train_images_count, &train_images_width, &train_images_height);
    uint8_t* train_labels_data = idx1_header_read(train_labels_raw, &train_labels_count);

    int image_data_size = train_images_height * train_images_width;
    printf("Image Data Size: %d\n", image_data_size);

    /***** NETWORK INITIALIZATION AND TRAINING ******/

    NeuralMatrix model = neural_matrix_create(4, 784, 128, 32, 10);
    neural_matrix_init(&model);
    neural_matrix_print_struct(&model);

    printf("Iterations: %d\n", iterations);
    printf("Learning Rate: %f\n", learning_rate);
    printf("Training network...\n");

    //train_images_count = 1;
    for (int i = 0; i < iterations; i++) {
        float cost = 0.0f;
        for (int j = 0; j < train_images_count; j++) {
            mnist_network_feed(&model, &train_images_data[j * image_data_size]);
            //neural_matrix_print(&model);
            neural_matrix_propagate_forward(&model);
            //neural_matrix_print(&model);
            set_desired_outputs(train_labels_data[j], &desired_outputs);
            cost += neural_matrix_compute_cost(&model, &desired_outputs);
            neural_matrix_propagate_backwards(&model, &desired_outputs);
            neural_matrix_update(&model, learning_rate);
        }
        if (i == 17) {
            learning_rate /= 10.0f;
            printf("New Learning Rate: %f\n", learning_rate);
        }
        cost /= train_images_count;
        printf("Epoch %d - Cost %f\n", i + 1, cost);
        //printf("Press Enter to continue;\n");
        //getchar();
    }
    free(train_images_raw);
    free(train_labels_raw);

    // **** TEST DATA *****

    uint8_t* test_images_raw = file_read(FILE_TEST_IMAGES);
    uint8_t* test_labels_raw = file_read(FILE_TEST_LABELS);
    int test_images_count, test_images_width, test_images_height, test_labels_count;
    uint8_t* test_images_data = idx3_header_read(test_images_raw, &test_images_count, &test_images_width, &test_images_height);
    uint8_t* test_labels_data = idx1_header_read(test_labels_raw, &test_labels_count);
    image_data_size = test_images_height * test_images_width;

    int med = 0;
    int imax, imin, omax, omin;
    float cost = 0.0f, min = 100.0f, max = -100.0f;
    for (int i = 0; i < test_images_count; i++) {
        mnist_network_feed(&model, &test_images_data[i * image_data_size]);
        neural_matrix_propagate_forward(&model);
        int out = check_output(&model);
        set_desired_outputs(test_labels_data[i], &desired_outputs);
        float c = neural_matrix_compute_cost(&model, &desired_outputs);
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

        if (out == (int)(unsigned int)test_labels_data[i]) med++;
    }
    float accuracy = (float)med * 100.0f / (float)test_images_count;
    printf("Test Accuracy: %f\n", accuracy);
    printf("Cost: %f\n", cost / (float)test_images_count);
    printf("Min Cost: Expected %d Desired: %u\n", omin, test_labels_data[imin]);
    printf("Max Cost: Expected: %d Desired: %u\n", omax, test_labels_data[imax]);
    idx3_to_png("min.png", test_images_data + imin * image_data_size, test_images_width, test_images_height);
    idx3_to_png("max.png", test_images_data + imax * image_data_size, test_images_width, test_images_height);
    system("open min.png");
    system("open max.png");

    //neural_matrix_print(&model);

    free(test_images_raw);
    free(test_labels_raw);
    neural_matrix_destroy(&model);
    return 0;
}

