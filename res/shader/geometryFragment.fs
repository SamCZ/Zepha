#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gSpecular;

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

uniform sampler2D tex;

void main() {
    gPosition = vec4(fragPos, 1);
    gNormal = vec4(normalize(normal), 1);
    gSpecular = texture(tex, texCoords);
}