#include <Nerv/Nerv.h>
#include <Imagine/Imagine.h>
#include <math.h>
#include <time.h>

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
    memset(array, 0, sizeof(float) * 10);
    array[u] = 1.0f;
}

static void forward_prop(nnetwork_t* network)
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
                neuron->activation = sigmoid(neuron->z);
            } else { //Sigmoid for output layer
                neuron->activation = sigmoid(neuron->z);
            }
        }
    }
}

static void network_init(nnetwork_t* network)
{
    for (int i = 0; i < network->layer_count - 1; i++) {
        layer_t* layer = network->layers + i;
        layer_t* next_layer = layer + 1;
        for (int j = 0; j < layer->neuron_count; j++) {
            neuron_t* neuron = layer->neurons + j;

            for (int k = 0; k < next_layer->neuron_count; k++) {
                neuron->output_weights[k] = (rand_weight() * 2.0f - 1.0f) / next_layer->neuron_count;
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

static int check_output(nnetwork_t* network)
{
    layer_t* output_layer = network->layers + network->layer_count - 1;
    float max = 0.0f;
    int index = 0;
    for (int i = 0; i < output_layer->neuron_count; i++) {
        neuron_t* neuron = output_layer->neurons + i;
        if (neuron->activation > max) {
            max = neuron->activation;
            index = i;
        }
    }
    return index;
}

static float calculate_cost(nnetwork_t* network, float* desired_outpus)
{
    float total_cost = 0.0f;
    layer_t* output_layer = network->layers + network->layer_count - 1;
    for (int i = 0; i < output_layer->neuron_count; i++) {
        neuron_t* neuron = output_layer->neurons + i;
        float cost = desired_outpus[i] - neuron->activation;
        total_cost += (cost * cost) * 0.5f;
    }
    return total_cost / output_layer->neuron_count;
}

static void back_prop(nnetwork_t* network, float* desired_output)
{
    layer_t* l = network->layers + network->layer_count - 1;
    layer_t* prev_layer = l - 1;

    // Output layer
    int i = 0;
    neuron_t* n = l->neurons;
    for (neuron_t* end = n + l->neuron_count; n != end; n++) {
        n->dz = (n->activation - desired_output[i]) * dsigmoid(n->z);
        
        neuron_t* prev_neuron = prev_layer->neurons;
        for (neuron_t* nend = prev_neuron + prev_layer->neuron_count; prev_neuron != nend; prev_neuron++) {
            prev_neuron->doutput_weights[i] = n->dz * prev_neuron->activation;
            prev_neuron->dactivation = prev_neuron->output_weights[i] * n->dz;
        }
        n->dbias = n->dz;
        i++;
    }

    --l;
    --prev_layer;

    // Hidden layers
    for (layer_t* lend = network->layers; l != lend; l--) {
        neuron_t* n = l->neurons;
        i = 0;
        for (neuron_t* nend = n + l->neuron_count; n != nend; n++) {
            n->dz = dsigmoid(n->z) * n->dactivation;

            neuron_t* prev_neuron = prev_layer->neurons;
            for (neuron_t* nnend = prev_neuron + prev_layer->neuron_count; prev_neuron != nnend; prev_neuron++) {
                prev_neuron->doutput_weights[i] = n->dz * prev_neuron->activation;   
                if (l == network->layers + 1) continue;
                prev_neuron->dactivation = prev_neuron->output_weights[i] * n->dz;
            }

            n->dbias = n->dz;
            i++;
        }
        prev_layer--;
    }
}

int main(int argc, char** argv)
{
    float learning_rate = 0.1f, desired_outputs[10];;
    int iterations = 10;
    if (argc > 1) iterations = atoi(argv[1]);

    srand(time(NULL));
    printf("Nerv Neural Network@\n");

    /***** TRAIN DATA ******/

    uint8_t* train_images_raw = file_read(FILE_TRAIN_IMAGES);
    uint8_t* train_labels_raw = file_read(FILE_TRAIN_LABELS);
    
    int train_images_count, train_images_width, train_images_height, train_labels_count;
    uint8_t* train_images_data = idx3_header_read(train_images_raw, &train_images_count, &train_images_width, &train_images_height);
    uint8_t* train_labels_data = idx1_header_read(train_labels_raw, &train_labels_count);

    int image_data_size = train_images_height * train_images_width;
    printf("Image Data Size: %d\n", image_data_size);

    /***** NETWORK INITIALIZATION AND TRAINING ******/

    //nnetwork_t network = nnetwork_load("nnsave.dat");
    nnetwork_t network = nnetwork_create(2, image_data_size, 10);
    network_init(&network);
    for (int i = 0; i < network.layer_count; i++) {
        printf("Layer: %d - Neurons: %d\n", i + 1, network.layers[i].neuron_count);
    }
    printf("Iterations: %d\n", iterations);
    printf("Learning Rate: %f\n", learning_rate);
    printf("Training network...\n");

    for (int i = 0; i < iterations; i++) {
        float cost = 0.0f, timer = timer_start();
        for (int j = 0; j < train_images_count; j++) {
            float t = timer_start();
            mnist_network_feed(&network, &train_images_data[image_data_size * j]);
            float tt = timer_start();
            //printf("Feed Time: %f\n", tt - t);
            //nnetwork_print_input(&network);
            t = timer_start();
            forward_prop(&network);
            tt = timer_start();
            //printf("Forward Time: %f\n", tt - t);
            //nnetwork_print_output(&network);
            t = timer_start();
            set_desired_outputs(train_labels_data[j], &desired_outputs[0]);
            tt = timer_start();
            //printf("Set Expected Output Time: %f\n", tt - t);

            t = timer_start();
            cost += calculate_cost(&network, &desired_outputs[0]);
            tt = timer_start();
            //printf("Calculate Cost Time: %f\n", tt - t);
            t = timer_start();
            back_prop(&network, &desired_outputs[0]);
            tt = timer_start();
            //printf("Backwards Time: %f\n", tt - t);
            t = timer_start();
            nnetwork_update(&network, learning_rate);
            tt = timer_start();
            //printf("Update Time: %f\n", tt - t);
            //nnetwork_print(&network);
        }
        printf("Iteration %d cost: %f %f\n", i + 1, cost, cost / (float)train_images_count);
        printf("Time: %f\n", timer_end(timer));
    }

    nnetwork_save("nnsave.dat", &network);
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
        mnist_network_feed(&network, &test_images_data[i * image_data_size]);
        forward_prop(&network);
        int out = check_output(&network);
        //nnetwork_print_output(&network);
        //printf("Expected Label: %d\n", out);
        //printf("Desired Label: %u\n", test_labels_data[i]);
        set_desired_outputs(test_labels_data[i], &desired_outputs[0]);
        float c = calculate_cost(&network, &desired_outputs[0]);
        //printf("Cost: %f\n", c);
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

    printf("Min Cost: Expected %d Desired: %u\n", omin, test_labels_data[imin]);
    printf("Max Cost: Expected: %d Desired: %u\n", omax, test_labels_data[imax]);
    idx3_to_png("min.png", test_images_data + imin * image_data_size, test_images_width, test_images_height);
    idx3_to_png("max.png", test_images_data + imax * image_data_size, test_images_width, test_images_height);
    system("open min.png");
    system("open max.png");

    nnetwork_destroy(&network);
    free(test_images_raw);
    free(test_labels_raw);
}