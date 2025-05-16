// clang-format off
#include <cmath>
#include <iostream>

// third-party
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"

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
    // always makes the window floating for i3wm
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // Use OpenGL core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
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

    // handle window resize
    glfwSetFramebufferSizeCallback(window, handleWindowResizeOpenGL);

    // NOTE: BUFFER VERTEX BUFFER STUFF START HERE

    // clang-format off
    float VertexData[] = {
        // positions        //colors          // texture cords
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 1.0f,       // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  1.0f, 1.0f,       // top-right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,       // bottom-left
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  1.0f, 0.0f,       // bottom-right
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
    // pos-vertex-attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color-vertex-attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture-coordinate-vertex-attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // unbinding the VAO (rebind when needed)
    glBindVertexArray(0);
    // unbinding the EBO (rebind when need)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // NOTE: BUFFER VERTEX BUFFER STUFF END HERE

    // creating shader program (with vertex and fragment shader)
    Shader* ourShader = new Shader(
        "/home/harsh/Desktop/learning_opengl/application/assets/shader/vertexShader.glsl",
        "/home/harsh/Desktop/learning_opengl/application/assets/shader/"
        "fragmentShader.glsl");

    // for wireframe mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // creating texture
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // setting texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading texture
    int width, height, nrChannels;
    unsigned char* textureData = stbi_load(
        "/home/harsh/Desktop/learning_opengl/application/assets/texture/wall.jpg", &width,
        &height, &nrChannels, 0);
    if(!textureData) {
        std::cerr << "ERROR: Failed To Load Texture File" << std::endl;
        return -1;
    }
    // loading texture in GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free texture data after loading it in GPU
    stbi_image_free(textureData);

    // creating texture
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // setting texture wrapping/filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // loading texture
    textureData = stbi_load(
        "/home/harsh/Desktop/learning_opengl/application/assets/texture/container.jpg",
        &width, &height, &nrChannels, 0);
    if(!textureData) {
        std::cerr << "ERROR: Failed To Load Texture File" << std::endl;
        return -1;
    }
    // loading texture in GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                 textureData);
    glGenerateMipmap(GL_TEXTURE_2D);
    // free texture data after loading it in GPU
    stbi_image_free(textureData);

    // using shader to set uniforms
    ourShader->Use();
    ourShader->setUniformInt("texture1", 0);
    ourShader->setUniformInt("texture2", 1);

    unsigned int transformLoc = glGetUniformLocation(ourShader->Program_ID, "tranform");

    // main loop
    while(!glfwWindowShouldClose(window)) {
        // handleing input events
        processInput(window);

        // both the glClear functions target the back buffer
        glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
        // despite its name glClear actually fill the buffer with the GL_COLOR_BUFFER_BIT
        // we just set
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures to corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glm::mat4 transformationMtx(1.0f); // make the identity matrix
        transformationMtx =
            glm::rotate(transformationMtx, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        transformationMtx = glm::translate(transformationMtx, glm::vec3(0.5, -0.5, 1.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transformationMtx));

        /* RENDER HERE */
        ourShader->Use();
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        // poll for any new event
        glfwPollEvents();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);

        // moves the triangle on x-axis till it reaches end of screen
    }

    // cleanup OpenGL and GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete ourShader;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
