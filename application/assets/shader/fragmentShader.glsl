#version 460 core

in vec2 texCoords;

out vec4 FragColor;

layout(location = 0) uniform sampler2D containerTexture;
layout(location = 1) uniform sampler2D catTexture;

void main()
{
    FragColor = mix(texture(containerTexture, texCoords), texture(catTexture, texCoords), 0.7);
};
