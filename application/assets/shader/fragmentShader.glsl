#version 460 core

layout(location = 0) in vec3 color;
layout(location = 1) in vec2 textureCordsIn;

layout(location = 0) out vec4 FragColor;

layout(location = 0) uniform sampler2D textureAtlas;

void main()
{
    vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCordsIn), 0);

    if (textureColor.a == 0.0) {
        discard;
    }

    FragColor = textureColor;
};
