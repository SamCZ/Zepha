#version 330 core

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gSpecular;

in vec2 texCoords;
in vec2 blendMaskCoords;
in vec3 blend;
in vec3 fragPos;
in vec3 normal;
in vec3 light;

uniform float time;

uniform sampler2D tex;

void main() {
    float blendMaskMult = -1;

    if (blendMaskCoords.x >= 0 && blendMaskCoords.y >= 0) blendMaskMult = texture(tex, blendMaskCoords).r;

    vec4 spec = texture(tex, texCoords);
    vec3 blendCol = blend;
    if (blendMaskMult >= 0) {
        blendCol = (vec3(1, 1, 1) * (1 - blendMaskMult)) + (blendCol * blendMaskMult);
    }

    spec = vec4(spec.xyz * blendCol * light, spec.a);

    if (spec.a < 0.1) discard;

    gSpecular = spec;
    gPosition = vec4(fragPos, 1);
    gNormal = vec4(normal, 1);
}