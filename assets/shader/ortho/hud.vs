#version 330 core

layout (location = 0) in vec2  aPos;
layout (location = 1) in vec4  aColorData;
layout (location = 2) in vec3  aColorBlend;
layout (location = 3) in float aUseTex;

uniform mat4 model;
uniform mat4 ortho;

out vec4  colorData;
out vec3  colorBlend;
out float useTex;

void main() {
    gl_Position = ortho * model * vec4(aPos, 0, 1);
    colorData = aColorData;
    colorBlend = aColorBlend;
    useTex = aUseTex;
}
