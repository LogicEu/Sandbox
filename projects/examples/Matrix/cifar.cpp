#include <Matrix/Matrix.h>
#include <Imagine/Imagine.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_DATA_1 "assets/CIFAR10/data_batch_1.bin"
#define FILE_DATA_2 "assets/CIFAR10/data_batch_2.bin"
#define FILE_DATA_3 "assets/CIFAR10/data_batch_3.bin"
#define FILE_DATA_4 "assets/CIFAR10/data_batch_4.bin"
#define FILE_DATA_5 "assets/CIFAR10/data_batch_5.bin"
#define FILE_TEST "assets/CIFAR10/test_batch.bin"

#define DATASET_UNIT 3073
#define RED_OFFSET 0
#define GREEN_OFFSET 1024
#define BLUE_OFFSET 2048

static char* label_names[10] = {
    "airplane",
    "automobile",
    "bird",
    "cat",
    "deer",
    "dog",
    "frog",
    "horse",
    "ship",
    "truck"
};

static char* label_name(uint8_t label)
{
    return label_names[label];
}

static uint8_t* file_read(char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not read file '%s'\n", path);
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    uint8_t* buffer = (uint8_t*)malloc(size);
    fread(buffer, 1, size, file);
    fclose(file);
    return buffer;
}

static uint8_t label_at(uint8_t* buffer, int index)
{
    return buffer[index * DATASET_UNIT];
}

static uint8_t* image_at(uint8_t* buffer, int index)
{
    return buffer + index * DATASET_UNIT + 1;
}

static bmp_t bmp_cifar(uint8_t* img)
{
    bmp_t bmp = bmp_new(32, 32, 3);
    for (int y = 0; y < bmp.height; y++) {
        for (int x = 0; x < bmp.width; x++) {
            uint8_t p[3];
            p[0] = img[RED_OFFSET + y * bmp.width + x];
            p[1] = img[GREEN_OFFSET + y * bmp.width + x];
            p[2] = img[BLUE_OFFSET + y * bmp.width + x];
            memcpy(px_at(&bmp, x, y), &p, bmp.channels);
        }
    }
    return bmp;
}

static bmp_t bmp_cifar_greyscale(uint8_t* img)
{  
    bmp_t tmp = bmp_cifar(img);
    bmp_t bmp = bmp_greyscale(&tmp);
    bmp_free(&tmp);
    return bmp;
}

int main(int argc, char** argv)
{
    /*int index = 0;
    if (argc > 1) index = atoi(argv[1]);
    printf("CIFAR-10 image format test\n");

    uint8_t* buffer = file_read(FILE_DATA_1);

    uint8_t* img = image_at(buffer, index);
    uint8_t label = label_at(buffer, index);

    bmp_t tmp = bmp_cifar(img);
    bmp_t print_bmp = bmp_black_and_white(&tmp);
    bmp_free(&tmp);

    bmp_write("test.jpg", &print_bmp);
    bmp_free(&print_bmp);

    //CIFAR TEST
    bmp_t bmp = bmp_cifar_greyscale(img);
    printf("Chanels: %u\n", bmp.channels);
    tmp = bmp_conv2d(&bmp);
    bmp_write("conv.jpg", &tmp);
    bmp_free(&tmp);

    printf("Channels: %u Width: %u Height: %u\n", bmp.channels, bmp.width, bmp.height);
    printf("Label: %s\n", label_name(label));
    system("open test.jpg");
    system("open conv.jpg");

    bmp_free(&bmp);
    free(buffer);*/

    bmp_t src = bmp_load("image.png");
    printf("Source Width: %u Height: %u Channels: %u\n", src.width, src.height, src.channels);

    bmp_t img = bmp_greyscale(&src);
    bmp_free(&src);
    //src = bmp_channels_expand(&img, 3);

    src = bmp_conv2d(&img);
    printf("Dest Width: %u Height: %u Channels: %u\n", src.width, src.height, src.channels);
    
    bmp_write("test.png", &src);
    system("open image.png");
    system("open test.png");

    bmp_free(&img);
    bmp_free(&src);

    return 0;
}