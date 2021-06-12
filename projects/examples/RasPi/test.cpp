#include <stdio.h>
#include <Core/Core.h>

int main()
{
    if (!glfwInit()) {
        printf("GLFW Library could not initiate.\n");
        return -1;
    }

    GLFWwindow* window;
    //glfwWindowHint(GLFW_SAMPLES, 4);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(640, 480, "Core", NULL, NULL);

    if (!window) {
        printf("Could not initiate window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        printf("GLEW Library could not be initiated.\n");
        glfwTerminate();
        return -1;
    }

    if (GLEW_VERSION_3_2) printf("OpenGL 3.2 is supportted.\n");
    else printf("OpenGL 3.2 is not supported.\n");

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthFunc(GL_LESS);

    unsigned int shader = shader_load("assets/shaders/textures.vs", "assets/shaders/textures.fs");
    unsigned int quad = vertex_array_buffer_quad_create();

    

    //MAIN LOOP
    glClearColor(1.0, 0.0, 0.0, 1.0);
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
