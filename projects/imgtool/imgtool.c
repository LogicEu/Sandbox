#include <Imagine/Imagine.h>
#include <stdio.h>
#include <time.h>

typedef enum {
    COMMAND_NULL,
    COMMAND_BLACK_AND_WHITE,
    COMMAND_NEGATIVE,
    COMMAND_CUT,
    COMMAND_ROTATE,
    COMMAND_FLIP_HORIZONTAL,
    COMMAND_FLIP_VERTICAL,
    COMMAND_SCALE_UP,
    COMMAND_SCALE_DOWN,
    COMMAND_WHITE_TO_TRANSPARENT,
    COMMAND_WHITE_SENSIBILITY,
    COMMAND_JCOMPRESS,
    COMMAND_DUMP,
    COMMAND_FRAME_DUMP,
    COMMAND_RESIZE_WIDTH,
    COMMAND_RESIZE_HEIGHT,
    COMMAND_RESIZE_F
} CommandEnum;

static unsigned int jcompress_quality = 100;
static unsigned int sensibility = 255;
static unsigned int resize_x, resize_y;
static float resize_scale;

static char* output_num_string(const char* output_path, unsigned int num)
{
    const unsigned int size = strlen(output_path);
    if (size < 5) return NULL;

    char* ret = (char*)malloc(256);
    memcpy(ret, output_path, size - 4);
    ret[size - 4] = '\0';

    char num_str[32];
    sprintf(num_str, "%03d", num);
    
    strcat(ret, num_str);
    strcat(ret, &output_path[size - 4]);
    return ret;
}

static void command_execute(unsigned int command, bmp_t* bitmap)
{
    switch (command) {
        case COMMAND_BLACK_AND_WHITE: {
            bmp_t b = bmp_black_and_white(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_NEGATIVE: {
            bmp_t b = bmp_negative(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
	    break;
        }
        case COMMAND_FLIP_HORIZONTAL: {
            bmp_t b = bmp_flip_horizontal(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_FLIP_VERTICAL: {
            bmp_t b = bmp_flip_vertical(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_ROTATE: {
            bmp_t b = bmp_rotate(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_SCALE_UP: {
            bmp_t b = bmp_scale(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_SCALE_DOWN: {
            bmp_t b = bmp_reduce(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;  
        }
        case COMMAND_WHITE_TO_TRANSPARENT: {
            bmp_t b = bmp_white_to_transparent(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_WHITE_SENSIBILITY: {
            bmp_t b = bmp_clear_to_transparent(bitmap, (uint8_t)sensibility);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_CUT: {
            bmp_t b = bmp_cut(bitmap);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_JCOMPRESS: {
            bmp_t b = bmp_jcompress(bitmap, jcompress_quality);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_RESIZE_WIDTH: {
            bmp_t b = bmp_resize_width(bitmap, resize_x);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_RESIZE_HEIGHT: {
            bmp_t b = bmp_resize_height(bitmap, resize_y);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        case COMMAND_RESIZE_F: {
            bmp_t b = bmp_scale_lerp(bitmap, resize_scale);
            bmp_free(bitmap);
            memcpy(bitmap, &b, sizeof(bmp_t));
            break;
        }
        default: { break; }
    }
}

static void frame_dump(unsigned char* img, unsigned int width, unsigned int height, unsigned int channels)
{
    printf("Width: %u, Height: %u, Channels: %u\n", width, height, channels);
    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            printf("(%u", *(img + (width * y + x) * channels));
            for (unsigned int i = 1; i < channels; i++) {
                printf("-%u", *(img + (width * y + x) * channels + i));
            }
            printf(") ");
        }
        printf("\n");
    }
}

static long get_file_size(const char* path)
{
    FILE* file = fopen(path, "rb");
    if (!file) {
        printf("Could not open file '%s'\n", path);
        return 0;
    }
    fseek(file, 0, SEEK_END);
    long ret = ftell(file);
    fclose(file);
    return ret;
}

static void dump_file(bmp_t* bitmap, const char* path)
{
    if (!strlen(path)) return;
    long size = get_file_size(path);
    printf("File Name:\t'%s'\n", path);
    printf("File Size:\t%ldKb\t (%ldBytes)\n", size >> 10, size);
    printf("Image Width:\t%u px\n", bitmap->width);
    printf("Image Height:\t%u px\n", bitmap->height);
    printf("Image Channels:\t%u\n", bitmap->channels);
}

static void print_help()
{
    printf("\n**** IMGTOOL: COMMAND LINE HANDY IMAGE TOOL ****\n\n");
    printf("Enter up to 256 input image files and up to 256 commands to execute.\n");
    printf("Apply every command to each input image secuentially to perform fast and easy operations.\n");
    printf("Supported formats are PNG, JPG, GIF and PPM.\n");
    printf("The next is a simple use case example:\n\n");
    printf("$ imgtool input.png -o output.jpg\n\n");
    printf("This creates a copy of the input PNG in a JPG image format.\n");
    printf("Another useful example is:\n\n");
    printf("$ imgtool *.png -I -s -bw\n\n");
    printf("In this case we take all PNG files in the current directory and apply\n");
    printf("a scale-down and a grey scale effect to the original input files.\n");
    printf("The following are the commands and flags supported.\n\n");

    printf("'-o':\t\tWrite the output image file to the path and format following this flag.\n");
    printf("'-I':\t\tWrite the output to the same file path and format as input.\n");
    printf("'-n'\t\tNo output.\n");
    printf("'-d':\t\tDump main info about image file.\n");
    printf("'-D':\t\tDump image frame buffer as RGB/A values.\n");
    printf("'-j':\t\tCompress image using JPEG compression (lossy).\n");
    printf("'-bw':\t\tTransform to black and white.\n");
    printf("'-N':\t\tTransform to negative RGB values.\n");
    printf("'-cut':\t\tCut corners of the image when they are transparent.\n");
    printf("'-r':\t\tRotate by 90 degrees.\n");
    printf("'-S':\t\tScale up image by factor of two (nearest).\n");
    printf("'-s':\t\tScale down image by factor of two (linear).\n");
    printf("'-fh':\t\tFlip the image horizontally.\n");
    printf("'-fv'\t\tFlip the image vertically.\n");
    printf("'-Rx':\tResize width of image to specified width.\n");
    printf("'-Ry':\tResize height of image to specified height.\n");
    printf("'-R':\tResize scale of image to specified floating point number.\n");
    printf("'-t'\t\tSet white to transparent. Needs alpha channel present.\n");
    printf("'-T'\t\tSet clear colors to transparent with a sensibility between 0 and 255.\n");
    printf("'-q':\t\tSet quality for JPEG compression output when writing to JPG.\n");
    printf("'-to-gif':\tWrite output images to a single output GIF file.\n");
    printf("'-from-gif':\tTake every frame of input GIF file as input images.\n");
    printf("'-open':\tOpen the first output image after process is completed.\n");
}

int main(int argc, char** argv)
{
    char input_path[256][256], output_path[256] = "output.png";
    unsigned int commands[256], command_count = 0;
    unsigned int output_count = 1, input_count = 0, output_to_gif = 0, input_from_gif = 0;
    unsigned int output_to_input = 0, open_at_exit = 0;

    if (argc <= 1) {
        printf("Missing input file and input command. Use -help to see instructions.\n");
        return 0;
    }

    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-help")) {
            print_help();
            return 0;
        } 
        else if (!strcmp(argv[i], "-o") && i + 1 < argc) {
            strcpy(output_path, argv[++i]);
            command_count++;
        }
        else if (!strcmp(argv[i], "-q") && i + 1 < argc) {
            img_set_jpeg_quality(atoi(argv[++i]));
        }
        else if (!strcmp(argv[i], "-Rx") && i + 1 < argc) {
            commands[command_count++] = COMMAND_RESIZE_WIDTH;
            resize_x = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-Ry") && i + 1 < argc) {
            commands[command_count++] = COMMAND_RESIZE_HEIGHT;
            resize_y = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-R") && i + 1 < argc) {
            commands[command_count++] = COMMAND_RESIZE_F;
            resize_scale = atof(argv[++i]);
        }
        else if (!strcmp(argv[i], "-T") && i + 1 < argc) {
            commands[command_count++] = COMMAND_WHITE_SENSIBILITY;
            sensibility = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-n")) {
            output_count = 0;
            command_count++;
        }
        else if (!strcmp(argv[i], "-N")) {
            commands[command_count++] = COMMAND_NEGATIVE;
        }
        else if (!strcmp(argv[i], "-I")) {
            output_to_input = 1;
        }
        else if (!strcmp(argv[i], "-cut")) {
            commands[command_count++] = COMMAND_CUT;
        }
        else if (!strcmp(argv[i], "-d")) {
            commands[command_count++] = COMMAND_DUMP;
        } 
        else if (!strcmp(argv[i], "-D")) {
            commands[command_count++] = COMMAND_FRAME_DUMP;
        }
        else if (!strcmp(argv[i], "-bw")) {
            commands[command_count++] = COMMAND_BLACK_AND_WHITE;
        }
        else if (!strcmp(argv[i], "-t")) {
            commands[command_count++] = COMMAND_WHITE_TO_TRANSPARENT;
        }
        else if (!strcmp(argv[i], "-r")) {
            commands[command_count++] = COMMAND_ROTATE;
        }
        else if (!strcmp(argv[i], "-S")) {
            commands[command_count++] = COMMAND_SCALE_UP;
        }
        else if (!strcmp(argv[i], "-s")) {
            commands[command_count++] = COMMAND_SCALE_DOWN;
        }
        else if (!strcmp(argv[i], "-to-gif")) {
            output_to_gif = 1;
        }
        else if (!strcmp(argv[i], "-from-gif")) {
            input_from_gif = 1;
        }
        else if (!strcmp(argv[i], "-open")) {
            open_at_exit = 1;
        }
        else if (!strcmp(argv[i], "-j") && i + 1 < argc) {
            commands[command_count++] = COMMAND_JCOMPRESS;
            jcompress_quality = atoi(argv[++i]);
        }
        else if (!strcmp(argv[i], "-fh")) {
            commands[command_count++] = COMMAND_FLIP_HORIZONTAL;
        }
        else if (!strcmp(argv[i], "-fv")) {
            commands[command_count++] = COMMAND_FLIP_VERTICAL;
        }  else strcpy(input_path[input_count++], argv[i]);

        if (command_count == 255 || input_count == 255) break;
    }

    /*********
     * FAIL
     * ******/
    
    if (!input_count) {
        printf("Missing input image file. See -help for more info.\n");
        return 0;
    }
    if (!command_count) {
        printf("Missing command to execute. See -help for more info.\n");
        return 0;
    }

    /************************
     * LOAD INPUT IMAGE FILES
     * *********************/

    bmp_t* bitmaps;
    if (input_from_gif) {
        gif_t* g = gif_file_load(input_path[0]);
        bitmaps = gif_to_bmp(g, &input_count);
        gif_free(g);
    } else {
        bitmaps = (bmp_t*)malloc(input_count * sizeof(bmp_t));
        for (unsigned int i = 0; i < input_count; i++) {
            bitmaps[i] = bmp_load(input_path[i]);
        }
    }

    /*******************************
     * APPLY COMMANDS AND OPERATIONS
     * ****************************/

    for (unsigned int i = 0; i < input_count; i++) {
        for (unsigned int j = 0; j < command_count; j++) {
            if (commands[j] == COMMAND_DUMP) dump_file(&bitmaps[i], input_path[i]);
            if (commands[j] == COMMAND_FRAME_DUMP) frame_dump(bitmaps[i].pixels, bitmaps[i].width, bitmaps[i].height, bitmaps[i].channels);
            else command_execute(commands[j], &bitmaps[i]);
        }
    }

    /********************
     * WRITE OUTPUT FILES
     * ******************/

    char open_str[256] = "open ";

    if (output_to_gif) {
        gif_t* g = bmp_to_gif(bitmaps, input_count);
        gif_file_write(output_path, g);
        gif_free(g);
        if (open_at_exit) {
            strcat(open_str, output_path);
            system(&open_str[0]);
        }
    }
    else if (output_to_input) {
        for (unsigned int i = 0; i < input_count; i++) {
            bmp_write(input_path[i], &bitmaps[i]);
            bmp_free(&bitmaps[i]);
        }
        if (open_at_exit) {
            strcat(open_str, input_path[0]);
            system(&open_str[0]);
        }
    } else if (output_count) {
        if (input_count > 1) {
            for (unsigned int i = 0; i < input_count; i++) {
                char* output_path_num = output_num_string(output_path, i);
                bmp_write(output_path_num, &bitmaps[i]);
                bmp_free(&bitmaps[i]);
            }
            if (open_at_exit) {
                strcat(open_str, output_num_string(output_path, 0));
                system(&open_str[0]);
            }
        } else {
            bmp_write(output_path, bitmaps);
            bmp_free(bitmaps);
            if (open_at_exit) {
                strcat(open_str, output_path);
                system(&open_str[0]);
            }
        }
    } 
    free(bitmaps);
    return 0;
}
