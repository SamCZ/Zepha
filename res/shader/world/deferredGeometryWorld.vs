#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec2  aTexCoords;
layout (location = 2) in vec3  aNormal;
layout (location = 3) in float aMod;
layout (location = 4) in vec3  aModValue;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);

    fragPos = worldPos.xyz;
    texCoords = aTexCoords;
    normal = aNormal;

    gl_Position = projection * view * worldPos;
}
