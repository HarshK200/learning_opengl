#include <iostream>

// third-party
#include "glad/glad.h"

// project includes
#include "Renderer.h"

void GLClearError() {
    while(glGetError() != GL_NO_ERROR)
        ;
}

bool GLLogCall() {
    if(GLenum error = glGetError()) {
        std::cerr << "[OpenGL Error]: (" << error << ")" << std::endl;
        return false;
    }

    return true;
}
