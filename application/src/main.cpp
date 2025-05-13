#include "GLFW/glfw3.h"
#include <GL/gl.h>
#include <iostream>

int main() {
    if(!glfwInit()) {
        std::cerr << "Couldn't Initiaize GLFW" << std::endl;
        return -1;
    }

    std::cout << "Hello GLFW + OpenGL" << std::endl;
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

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // NOTE: doesn't  this glfwDestroyWindow() call is not really need since
    // glfwTerminate() destorys all window either way
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
