// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>
// clang-format on

// changes OpenGL viewport on resize
void handleWindowResizeOpenGL(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int main() {
    if(!glfwInit()) {
        std::cerr << "Couldn't Initiaize GLFW" << std::endl;
        return -1;
    }

    std::cout << "Hello GLFW + GLAD OpenGL" << std::endl;

    // always makes the window floating for i3wm
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Ballz", nullptr, nullptr);

    if(!window) {
        glfwTerminate();
        return -1;
    }

    // making window context current tells openGL that this window is to issue cammand to
    glfwMakeContextCurrent(window);

    // loading GLAD for modern OpenGL functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glViewport(0, 0, 800, 600);

    // Print OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    glfwSetFramebufferSizeCallback(window, handleWindowResizeOpenGL);

    // NOTE: modern OpenGL traingle drawing here

    // clang-format off
    float VertexData[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };
    // clang-format on

    // vertex buffer object in GPU memory
    unsigned int VertexBuffer;
    glGenBuffers(1, &VertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

    // telling how each vertex layout looks like
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // main loop
    while(!glfwWindowShouldClose(window)) {
        // handleing input events
        processInput(window);

        // both the glClear functions target the back buffer
        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        // despite its name glClear actually fill the buffer with the GL_COLOR_BUFFER_BIT
        // we just set
        glClear(GL_COLOR_BUFFER_BIT);

        /* RENDER HERE */
        glDrawArrays(GL_TRIANGLES, 0, 3);


        // poll for any new event
        glfwPollEvents();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);
    }

    // NOTE: doesn't  this glfwDestroyWindow() call is not really need since
    // glfwTerminate() destorys all window either way but is good practice
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
