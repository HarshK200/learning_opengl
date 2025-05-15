#pragma once

#include <glad/glad.h>
#include <string>

class Shader {
public:
    // shader program ID
    unsigned int Program_ID;

    // reads shader code from disk, compiles it, makes a shader program and links it
    Shader(const char* vertexCodePath, const char* fragmentCodePath);
    // clean up delete the program
    ~Shader();

    void Use();

    // utility functions to set uniforms
    void setUniformBool(const std::string& name, bool value) const;
    void setUniformInt(const std::string& name, int value) const;
    void setUniformFloat(const std::string& name, float value) const;
};
