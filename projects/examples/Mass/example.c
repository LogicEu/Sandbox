#include <Mass/Mass.h>

int main(int argc, char** argv)
{
    vmesh_t* vmesh = vmesh_shape_plane(30, 30);
    vmesh_move(vmesh, vec3_new(-15.0f, -2.5f, -15.0f));
    
    vmesh_t* cube = vmesh_shape_cube(4);
    vmesh_move(cube, vec3_new(4.0f, 2.5f, 4.0f));
    vmesh_combine(vmesh, cube);

    vmesh_t* sphere = vmesh_shape_sphere(6);
    vmesh_move(sphere, vec3_new(-4.0f, 2.5f, 4.0f));
    vmesh_combine(vmesh, sphere);

    vmesh_write_file_quick(vmesh, "scene_quick.obj");
    vmesh_write_file(vmesh, "scene.obj");
    vmesh_free(vmesh);
    return 0;
}
