#version 400 core

uniform mat4 modelViewMatrix;
uniform mat4 projectionMatrix;

layout (location = 0) in vec2 va_Vertex;
layout (location = 2) in vec2 va_Texcoord;

out vec2 texCoord;

void main()
{
    texCoord = va_Texcoord;
    gl_Position = projectionMatrix * modelViewMatrix * vec4(va_Vertex, 0.0, 1.0);
}
