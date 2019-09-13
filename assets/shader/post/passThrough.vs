#version 330 core

//Just translate the coordinates directly into screen space.
//For rendering the data onto a quad.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main() {
    texCoords = aTexCoords;
    gl_Position = vec4(aPos, 1.0);
}