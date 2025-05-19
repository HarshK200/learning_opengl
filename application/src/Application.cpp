// clang-format off
#include <cmath>
#include <iostream>

// third-party
#include "IndexBuffer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/ext/matrix_transform.hpp"
#include "stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Common.h"
#include "VertexBuffer.h"

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
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  0.0f, 16.0f,       // top-left
         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  32.0f, 16.0f,       // top-right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  0.0f, 48.0f,       // bottom-left
         0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,  32.0f, 48.0f,       // bottom-right
    };
    
    unsigned int indices[] = {
        0, 2, 3, 0, 1, 3, // quad 1
    };
    // clang-format on

    // vertex array object in GPU memory and config VAO
    unsigned int va;
    glGenVertexArrays(1, &va);

    // binding the VAO for future calls

    VertexBuffer* VBO = new VertexBuffer(VertexData, sizeof(VertexData));
    IndexBuffer* IBO = new IndexBuffer(indices, 6);

    glBindVertexArray(va);
    // Setting VAO config
    // pos-vertex-attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // goes to vertex shader layout 0 slot
    // color-vertex-attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // goes to vertex shader layout 1 slot
    // texture-coordinate-vertex-attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2); // goes to vertex shader layout 2 slot

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

    // NOTE: texture (pixel art)
    {
        int width, height, channels;
        unsigned char* data = stbi_load("/home/harsh/Desktop/learning_opengl/application/"
                                        "assets/texture/Texture_Atlas.png",
                                        &width, &height, &channels, 4);
        if(!data) {
            std::cerr << "ERROR: couldn't load image" << std::endl;
            ASSERT(data)
            return -1;
        }

        unsigned int textureAtlas;
        glGenTextures(1, &textureAtlas);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureAtlas);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // NOTE: load into GPU memory
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D); // disable mulitsampling

    // using shader to set uniforms
    ourShader->Use();

    // rotation
    // transformMatrix = glm::translate(transformMatrix, glm::vec3(1.0f, 1.0f, 1.0f));

    // main loop
    while(!glfwWindowShouldClose(window)) {
        // handleing input events
        processInput(window);

        // clear screen with color
        glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        /* RENDER HERE */

        // rotation baby!!
        ourShader->Use();
        glBindVertexArray(va);
        IBO->Bind();

        unsigned int uniformTransformID = glGetUniformLocation(ourShader->Program_ID, "transform");
        glm::mat4 transformMatrix = glm::mat4(1.0f);
        transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
        transformMatrix = glm::rotate(transformMatrix, (float)glfwGetTime(), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(uniformTransformID, 1, GL_FALSE, glm::value_ptr(transformMatrix));


        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

        // poll for any new event
        glfwPollEvents();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);

        // moves the triangle on x-axis till it reaches end of screen
    }

    // cleanup OpenGL and GLFW
    delete VBO;
    delete IBO;

    glDeleteVertexArrays(1, &va);
    delete ourShader;

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
