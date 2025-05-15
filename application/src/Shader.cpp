#include "Shader.h"

#include <GL/glext.h>
#include <glad/glad.h> // for OpenGL functions

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexCodePath, const char* fragmentCodePath) {

    // read shader code from disk
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vertexIF;
    std::ifstream fragmentIF;

    // by default ifstream doesn't throw execptions so this is telling it to do so
    vertexIF.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // opening the files
        vertexIF.open(vertexCodePath);
        fragmentIF.open(fragmentCodePath);

        // reading the file to string streams
        std::stringstream vertexStringStream, fragmentStringStream;
        vertexStringStream << vertexIF.rdbuf();
        fragmentStringStream << fragmentIF.rdbuf();

        // converting stringstreams to string and setting the code
        // NOTE: you cannot do something like .str().c_str() since c_str only returns a
        // pointer to the underlying string start and since you never set the string to a
        // variable it is lost and the c_str i.e. const char* is points to an invalid
        // memory address
        vertexCode = vertexStringStream.str();
        fragmentCode = fragmentStringStream.str();

        vertexIF.close();
        fragmentIF.close();
    } catch(std::ifstream::failure e) {
        std::cout << "ERROR: Reading shader file" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // compile the shaders
    unsigned int vertexShader;
    unsigned int fragmentShader;

    int success;
    char infoLog[512];

    // vertexShader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR: Compiling Vertex Shader:\n" << infoLog << std::endl;
    }

    // vertexShader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR: Compiling Fragment Shader:\n" << infoLog << std::endl;
    }

    // create shader porgram
    Program_ID = glCreateProgram();
    glAttachShader(Program_ID, vertexShader);
    glAttachShader(Program_ID, fragmentShader);

    // link the porgram with the shaders
    glLinkProgram(Program_ID);
    glGetProgramiv(Program_ID, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(Program_ID, 512, NULL, infoLog);
        std::cerr << "ERROR: Linking Shader Program:\n" << infoLog << std::endl;
    }

    // cleanup no longer required shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    // NOTE: cannot call after glfwDestoryWindow() or glfwTerminate();
    glDeleteProgram(Program_ID);
}

void Shader::Use() { glUseProgram(Program_ID); }

void Shader::setUniformBool(const std::string& name, bool value) const {
    unsigned int uniformLocation = glGetUniformLocation(Program_ID, name.c_str());
    glUniform1i(uniformLocation, (int)value);
}

void Shader::setUniformInt(const std::string& name, int value) const {
    unsigned int uniformLocation = glGetUniformLocation(Program_ID, name.c_str());
    glUniform1i(uniformLocation, value);
}

void Shader::setUniformFloat(const std::string& name, float value) const {
    unsigned int uniformLocation = glGetUniformLocation(Program_ID, name.c_str());
    glUniform1f(uniformLocation, value);
}
