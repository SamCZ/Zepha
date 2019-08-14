#version 420 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec4  aColorData;
layout (location = 2) in vec3  aColorBlend;
layout (location = 3) in float aUseTex;
layout (location = 4) in vec3  aNormal;

uniform mat4 model;
uniform mat4 ortho;

out float useTex;
out vec4 colorData;
out vec3 colorBlend;

void main() {
    gl_Position = ortho * model * vec4(aPos, 1.0);
    useTex = aUseTex;
    colorData = aColorData;
    colorBlend = aColorBlend;
}
