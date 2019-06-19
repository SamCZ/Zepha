#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec4  aColorData;
layout (location = 2) in float aUseTex;
layout (location = 3) in vec3  aNormal;

uniform mat4 model;
uniform mat4 ortho;

out float useTex;
out vec4 colorData;

out int texMode;

void main() {
    gl_Position = ortho * model * vec4(aPos, 1.0);
    useTex = aUseTex;
    colorData = aColorData;
}
