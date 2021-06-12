#include "../BNGame.h"

char* char_map_get_char(char** array, size_t offx, size_t offy)
{
    return *(array + offy) + offx;
}

char** char_map_init(size_t width, size_t height)
{
    char** map = (char**)malloc(sizeof(char*) * height);
    for (size_t y = 0; y < height; y ++) {
        *(map+y) = (char*)malloc(sizeof(char) * width);
        for (size_t x = 0; x < width; x ++) {
            char* ptr = char_map_get_char(map, x, y);   
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                *ptr = '#';
            }
            else if (rand() % 4 > 2) *ptr = '#';
            else *ptr = ' ';
        }
    }
    return map;
}

char** char_map_read_file(const char* path, unsigned int* widthptr, unsigned int* heightptr)
{
    unsigned int width = 0, height = 0;
    FILE* file = fopen(path, "r");
    while(1) {
        char c = fgetc(file);
        if (feof(file)) break;
        else if (c == '\n') height += 1;
        else if (height == 0) width += 1;
    }
    fclose(file);

    *widthptr = width;
    *heightptr = height;

    char** map = (char**)malloc(sizeof(char*) * height);
    for (size_t y = 0; y < height; y++) {
        *(map+y) = (char*)malloc(sizeof(char) * width);
    }
    
    file = fopen(path, "r");
    unsigned int x = 0, y = 0;
    while (1) {
        char c = fgetc(file);
        if (feof(file)) break;
        else if (c == '\n') {
            y++;
            x = 0;
        } else {
            *char_map_get_char(map, x, y) = c;
            x++;
        }
    }
    fclose(file);
    return map;
}

void char_map_write_file(char** map, size_t width, size_t height, const char* path)
{
    FILE* file;
    file = fopen(path, "w");
    for (size_t y = 0; y < height; y ++) {
        for (size_t x = 0; x < width; x ++) {
            fprintf(file, "%c", *(char_map_get_char(map, x, y)));
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void char_map_print(char** map, unsigned int width, unsigned int height)
{
    for (size_t y = 0; y < height; y ++) {
        for (size_t x = 0; x < width; x++) {
            printf("%c", *char_map_get_char(map, x, y));
        }
        printf("\n");
    }
}

void map_char_print_file(const char* path)
{
    char* cat = (char*)alloca(sizeof(char)*40);
    strcpy(cat, "cat ");
    strcat(cat, path);
    system(cat);
}

void char_map_free(char** map, size_t height)
{
    for (size_t y = 0; y < height; y++) {
        free(*(map+y));
    }        
    free(map);
}