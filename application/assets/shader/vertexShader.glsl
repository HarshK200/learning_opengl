#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 textureCoordinates;

out vec2 texCoords;

uniform mat4 tranformMatrix;

void main()
{
    // gl_Position = vec4(position, 1.0);
    gl_Position = tranformMatrix * vec4(position, 1.0);

    texCoords = textureCoordinates;
};
