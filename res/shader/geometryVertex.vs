#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in float aUseTex; /* Unused */
layout (location = 2) in vec4  aTexCoords;
layout (location = 3) in vec3  aNormal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 texCoords;
out vec3 fragPos;
out vec3 normal;

void main() {
    vec4 modelViewPos = view * model * vec4(aPos, 1.0);

    fragPos = modelViewPos.xyz;
    texCoords = aTexCoords.xy;
    normal = transpose(inverse(mat3(model))) * aNormal;

    gl_Position = projection * modelViewPos;
}
