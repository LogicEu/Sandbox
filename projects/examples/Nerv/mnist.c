#include <Nerv/Nerv.h>
#include <Imagine/Imagine.h>
#include <math.h>

#define FILE_TRAIN_IMAGES "assets/MNIST/train-images-idx3-ubyte"
#define FILE_TRAIN_LABELS "assets/MNIST/train-labels-idx1-ubyte"
#define FILE_TEST_IMAGES "assets/MNIST/t10k-images-idx3-ubyte"
#define FILE_TEST_LABELS "assets/MNIST/t10k-labels-idx1-ubyte"

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
    bmp_write("test.png", &bmp);
    bmp_free(&bmp);
}

void mnist_network_feed(nnetwork_t* network, uint8_t* image)
{
    layer_t* input_layer = network->layers;
    for (int i = 0; i < input_layer->neuron_count; i++) {
        neuron_t* neuron = input_layer->neurons + i;
        neuron->activation = utof(image[i]);
    }
}

void set_desired_outputs(uint8_t u, float* array)
{
    int k = (int)(unsigned int)u;
    for (int i = 0; i < 10; i++) {
        array[i] = 0.0f;
        if (i == k) array[i] = 1.0f;
    }
}

void forward_prop(nnetwork_t* network)
{
    for (int i = 1; i < network->layer_count; i++) {
        layer_t* layer = network->layers + i;
        layer_t* prev_layer = layer - 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;
            neuron->z = neuron->bias;

            for (int k = 0; k < prev_layer->neuron_count; k++) {
                neuron_t* prev_neuron = prev_layer->neurons + k;
                neuron->z += prev_neuron->output_weights[j] * prev_neuron->activation;
            }

            //Relu for hidden layers
            if (i < network->layer_count - 1) {
                neuron->activation = relu(neuron->z);
            } else { //Sigmoid for output layer
                neuron->activation = sigmoid(neuron->z);
            }
        }
    }
}

void network_init(nnetwork_t* network)
{
    for (int i = 0; i < network->layer_count - 1; i++) {
        layer_t* layer = network->layers + i;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;

            for (int k = 0; k < next_layer->neuron_count; k++) {
                neuron->output_weights[k] = rand_weight() * 2.0f / layer->neuron_count;
            }

            if (!i) continue;
            neuron->bias = 0.0f;
        }
    }

    layer_t* last_layer = network->layers + network->layer_count - 1;
    for (int i = 0; i < last_layer->neuron_count; i++) {
        neuron_t* neuron = last_layer->neurons + i;
        neuron->bias = 0.0f;
    }
}

int main(int argc, char** argv)
{
    float learning_rate = 0.1f;
    int iterations = 10000;
    if (argc > 1) iterations = atoi(argv[1]);

    srand(time(NULL));

    /***** TRAIN DATA ******/

    uint8_t* train_images_raw = file_read(FILE_TRAIN_IMAGES);
    uint8_t* train_labels_raw = file_read(FILE_TRAIN_LABELS);
    
    int train_images_count, train_images_width, train_images_height, train_labels_count;
    uint8_t* train_images_data = idx3_header_read(train_images_raw, &train_images_count, &train_images_width, &train_images_height);
    uint8_t* train_labels_data = idx1_header_read(train_labels_raw, &train_labels_count);

    int image_data_size = train_images_height * train_images_width;
    printf("Image Data Size: %d\n", image_data_size);

    /***** NETWORK INITIALIZATION AND TRAINING ******/

    float desired_outputs[10];
    nnetwork_t network = nnetwork_load("nnsave.dat");
    /*nnetwork_t network = nnetwork_create(3, image_data_size, 32, 10);
    network_init(&network);
    for (int i = 0; i < network.layer_count; i++) {
        printf("Layer: %d - Neurons: %d\n", i + 1, network.layers[i].neuron_count);
    }
    printf("Iterations: %d\n", iterations);
    printf("Learning Rate: %f\n", learning_rate);
    printf("Training network...\n");
    //train_images_count = 1;
    for (int i = 0; i < iterations; i++) {
        for (int j = 0; j < train_images_count; j++) {
            mnist_network_feed(&network, &train_images_data[image_data_size * j]);
            //nnetwork_print_input(&network);
            forward_prop(&network);
            //nnetwork_print_output(&network);
            set_desired_outputs(train_labels_data[j], &desired_outputs[0]);
            //printf("Cost: %f\n", compute_cost(&network, &desired_outputs[0]));
            backwards_propagation(&network, &desired_outputs[0]);
            nnetwork_update(&network, learning_rate);
            //nnetwork_print(&network);
        }
        printf("Cost: %f\n", compute_cost(&network, &desired_outputs[0]));
        printf("Iteration %d\n", i);
    }
    nnetwork_save("nnsave.dat", &network);*/
    // **** TEST DATA *****

    uint8_t* test_images_raw = file_read(FILE_TEST_IMAGES);
    uint8_t* test_labels_raw = file_read(FILE_TEST_LABELS);
    int test_images_count, test_images_width, test_images_height, test_labels_count;
    uint8_t* test_images_data = idx3_header_read(test_images_raw, &test_images_count, &test_images_width, &test_images_height);
    uint8_t* test_labels_data = idx1_header_read(test_labels_raw, &test_labels_count);
    image_data_size = test_images_height * test_images_width;
    printf("Image Data Size: %d\n", image_data_size);
    
    int test_index;
    printf("Enter index of example to test (0 - %d): \n", test_images_count - 1);
    scanf("%d", &test_index);

    mnist_network_feed(&network, &test_images_data[test_index * image_data_size]);
    forward_propagation(&network);
    //nnetwork_print(&network);
    nnetwork_print_output(&network);

    printf("Label: %u\n", test_labels_data[test_index]);
    idx3_to_png("test.png", test_images_data + test_index * image_data_size, test_images_width, test_images_height);
    system("open test.png");

    nnetwork_destroy(&network);
    free(test_images_raw);
    free(test_labels_raw);
}