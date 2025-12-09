#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aUV;
out vec3 outColor;
out vec2 outUV;
uniform vec2 offset;

void main()
{
    vec3 pos = aPos/2 + vec3(offset, 0.0);
    gl_Position = vec4(pos, 1.0);
    outColor = aColor;
    outUV = aUV;
}