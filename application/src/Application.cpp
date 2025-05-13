#include "GL/gl.h"
#include "GLFW/glfw3.h"
#include <iostream>

int main() {
    if(!glfwInit()) {
        std::cerr << "Couldn't Initiaize GLFW" << std::endl;
        return -1;
    }

    std::cout << "Hello GLFW + OpenGL" << std::endl;
    std::cout << "\nGLFW VERSION DETAIL:\n" << glfwGetVersionString() << std::endl;
    GLFWwindow* window;
    window = glfwCreateWindow(1280, 720, "Ballz", nullptr, nullptr);

    if(!window) {
        glfwTerminate();
        return -1;
    }

    // making window context current tells openGL that this window is to issue cammand to
    glfwMakeContextCurrent(window);

    // main loop
    while(!glfwWindowShouldClose(window)) {
        // both the glClear functions target the back buffer
        glClearColor(0.8f, 0.9f, 1.0f, 1.0f);
        // despite its name glClear actually fill the buffer with the GL_COLOR_BUFFER_BIT
        // we just set
        glClear(GL_COLOR_BUFFER_BIT);

        // NOTE: drawing a traingle using legacy openGL 1.1(comes from GL/gl.h header
        // file) this is included in system by defualt

        glBegin(GL_TRIANGLES);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        // here be swap the front buffer with back buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // NOTE: doesn't  this glfwDestroyWindow() call is not really need since
    // glfwTerminate() destorys all window either way
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
