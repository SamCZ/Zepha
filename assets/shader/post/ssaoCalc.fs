#version 440 core

#define KERNELS 32
#define RADIUS 0.5
#define BIAS 0.04

out float outColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D texNoise;

uniform vec3 samples[KERNELS];

uniform mat4 projection;
uniform mat4 view;

float scale = 2;

const vec2 noiseScale = vec2(1920.0/4.0, 1004.0/4.0);

void main() {
    vec3 fragPos   = vec3(vec4(texture(gPosition, texCoords * scale).xyz, 1.0f));
    vec3 normal    = normalize(texture(gNormal, texCoords * scale).xyz);
    vec3 randomVec = normalize(texture(texNoise, texCoords * scale * noiseScale).xyz);

    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn       = mat3(tangent, bitangent, normal);

    vec4 fragSample = vec4(fragPos, 1.0);
    fragSample = projection * fragSample;
    fragSample.xyz /= fragSample.w;
    fragSample.xyz = fragSample.xyz * 0.5 + 0.5;

    float fragDepth = vec3(texture(gPosition, fragSample.xy)).z;

    if (fragDepth < -16) {
        outColor = 1;
    }
    else {
        float occlusion = 0.0;
        for (int i = 0; i < KERNELS; ++i) {
            vec3 smpl = tbn * samples[i]; // From tangent to view-space
            smpl = fragPos + smpl * RADIUS;

            vec4 offset = vec4(smpl, 1.0);
            offset = projection * offset;
            offset.xyz /= offset.w;
            offset.xyz = offset.xyz * 0.5 + 0.5;

            float sampleDepth = vec3(texture(gPosition, offset.xy)).z;

            float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(fragDepth - sampleDepth));
            occlusion += (sampleDepth >= fragDepth + BIAS ? 1.0 : 0.0) * rangeCheck;
        }

        outColor = 1.0 - (occlusion / KERNELS);
    }
}