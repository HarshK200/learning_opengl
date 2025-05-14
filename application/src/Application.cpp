// clang-format off
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <cmath>
#include <iostream>
// clang-format on

const char* vertexShaderCode = "#version 460 core\n"
                               "layout (location = 0) in vec3 aPos;\n"
                               "layout (location = 1) in vec3 aColor;\n"
                               "out vec3 color;"
                               "void main()\n"
                               "{\n"
                               "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                               "    color = aColor;\n"
                               "}\0";

// TODO: handle color
const char* fragmentShaderCode = "#version 460 core\n"
                                 "in vec3 color;\n"
                                 "out vec4 FragColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "    FragColor = vec4(color, 1.0);\n"
                                 "}\0";

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

    // NOTE: modern OpenGL traingle drawing data here

    // clang-format off
    float VertexData[] = {
        // positions        //colors
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,        // top-right
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,        // top-left
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,        // bottom-right
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,        // bottom-left
    };

    unsigned int indices[] = {
        0, 2, 3, // triangle 1
        0, 1, 3, // triangle 2
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

    // compiling shaders
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "Error compiling Vertex Shader:" << infoLog << std::endl;
        return -1;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "Error compiling Fragment Shader:" << infoLog << std::endl;
        return -1;
    }

    // linking shaders
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error LINKING Shader Programm:" << infoLog << std::endl;
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // setting/activating the shader program
    glUseProgram(shaderProgram);
    int VertexColorLocation = glGetUniformLocation(shaderProgram, "myColor");

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
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // poll for any new event
        glfwPollEvents();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);
    }

    // cleanup OpenGL and GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
