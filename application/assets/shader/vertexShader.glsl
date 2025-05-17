#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;

layout(location = 0) out vec3 color;
layout(location = 1) out vec2 texCoords;

uniform mat4 tranform;

void main()
{
    // gl_Position = tranform * vec4(aPos, 1.0);
    gl_Position = vec4(aPos, 1.0);

    color = aColor;
    texCoords = aTexCoords;
};
