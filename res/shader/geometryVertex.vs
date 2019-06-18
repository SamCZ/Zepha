#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in float aUseTex;
layout (location = 2) in vec4  aColorData;
layout (location = 3) in vec3  aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out float useTex;
out vec4  colorData;
out vec3  fragPos;
out vec3  normal;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);

    useTex = aUseTex;
    fragPos = worldPos.xyz;
    colorData = aColorData;
    normal = transpose(inverse(mat3(model))) * normalize(aNormal);

    gl_Position = projection * view * worldPos;
}
