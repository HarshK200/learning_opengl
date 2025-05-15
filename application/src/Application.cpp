// clang-format off
#include "Shader.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <cmath>
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    window = glfwCreateWindow(800, 600, "Ballz", nullptr, nullptr);

    if(!window) {
        glfwTerminate();
        std::cout << "Couldn't create window" << std::endl;
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

    // NOTE: BUFFER VERTEX BUFFER STUFF START HERE

    // clang-format off
    float VertexData[] = {
        // positions        //colors
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,        // top-middle
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,        // bottom-left
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,        // bottom-right
    };

    unsigned int indices[] = {
        0, 1, 2, // triangle 1
    };
    // clang-format on

    // generate Element Buffer Object
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // vertex buffer object in GPU memory and VAO for its config
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // binding the VAO for future calls
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);

    // Setting VAO config
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // unbinding the VAO (rebind when needed)
    glBindVertexArray(0);
    // unbinding the EBO (rebind when need)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // NOTE: BUFFER VERTEX BUFFER STUFF END HERE

    // creating shader program (with vertex and fragment shader)
    // clang-format off
    Shader* ourShader = new Shader(
        "/home/harsh/Desktop/learning_opengl/application/assets/shader/vertexShader.glsl",
        "/home/harsh/Desktop/learning_opengl/application/assets/shader/fragmentShader.glsl");
    // clang-format on
    // use shader program
    ourShader->Use();

    // for wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // main loop
    while(!glfwWindowShouldClose(window)) {
        // handleing input events
        processInput(window);

        // both the glClear functions target the back buffer
        glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
        // despite its name glClear actually fill the buffer with the GL_COLOR_BUFFER_BIT
        // we just set
        glClear(GL_COLOR_BUFFER_BIT);

        /* RENDER HERE */
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0);

        // poll for any new event
        glfwPollEvents();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);
    }

    // cleanup OpenGL and GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete ourShader;
    ourShader = nullptr;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
