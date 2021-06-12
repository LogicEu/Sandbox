#include <Taxi/Taxi.h>

/*
-------------
 -> Skybox <- 
-------------
*/

char** skybox_load_file_paths(const char* pre_path)
{
    char** file_paths = (char**)malloc(sizeof(char*) * 6);
    for (int i = 0; i < 6; i++) {
        char* this_path = (char*)malloc(sizeof(char) * 128);
        strcpy(this_path, pre_path);

        if (i % 2 == 0) strcat(this_path, "_p");
        else strcat(this_path, "_n");

        if (i < 2) strcat(this_path, "x");
        else if (i < 4) strcat(this_path, "y");
        else strcat(this_path, "z");

        strcat(this_path, ".png");
        *(file_paths + i) = this_path;
    }
    return file_paths;
}

skybox_t* skybox_load(const char* path)
{
    skybox_t* skybox = (skybox_t*)malloc(sizeof(skybox_t));
    char** paths = skybox_load_file_paths(path);
    skybox->cubemap = texture_cubemap(paths);
    skybox->VAO = vertex_array_buffer_skybox_create();
    skybox_free_file_paths(paths);
    return skybox;
}

void skybox_free_file_paths(char** file_paths)
{
    for (int i = 0; i < 6; i++) {
        free(*(file_paths + i));
    }
    free(file_paths);
}