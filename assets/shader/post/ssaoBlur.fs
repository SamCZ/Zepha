#version 440 core

#define RAD 2
#define DIVIDEBY ((RAD * 2) * (RAD * 2))

out float outColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D ssaoInput;

uniform float sampleScale;

void main() {
    vec2 texelSize = 1.0 / vec2(textureSize(ssaoInput, 0));

    float result = 0.0;
    for (int x = -RAD; x < RAD; x++) {
        for (int y = -RAD; y < RAD; y++) {
            vec2 offset = vec2(float(x), float(y)) * texelSize;
            result += texture(ssaoInput, (texCoords * sampleScale) + offset).r;
        }
    }

    outColor = result / DIVIDEBY;
}