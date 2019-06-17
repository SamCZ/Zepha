#version 330 core

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

void main() {
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal  = texture(gNormal, texCoords).rgb;
    vec3 color   = texture(gColorSpec, texCoords).rgb;

    fragColor = vec4(normal, 1.0);
}