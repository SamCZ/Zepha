#version 440 core

#define KERNELS 64
#define RADIUS 0.5
#define BIAS 0.025

out float outColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D texNoise;

uniform vec3 samples[KERNELS];

uniform mat4 projection;
uniform mat4 view;

float scale = 0.5;

const vec2 noiseScale = vec2(1920.0/4.0, 1004.0/4.0);

void main() {
    vec3 fragPos   = vec3(view * vec4(texture(gPosition, texCoords / scale).xyz, 1.0f));
    vec3 normal    = normalize(mat3(view) * texture(gNormal, texCoords / scale).xyz);
    vec3 randomVec = normalize(texture(texNoise, texCoords / scale * noiseScale).xyz);

    if (fragPos == 0) discard;

    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn       = mat3(tangent, bitangent, normal);
//
//    vec3 smpl = tbn * samples[i]; // From tangent to view-space
//    smpl = fragPos + smpl * RADIUS;
//
//    vec4 offset = vec4(smpl, 1.0);
//    offset = projection * offset;
//    offset.xyz /= offset.w;
//    offset.xyz = offset.xyz * 0.5 + 0.5;
//
//    float sampleDepth = vec3(projection * view * texture(gPosition, offset.xy)).z;
//    float fragDepth = vec3(projection * view * texture(gPosition, vec4(projection * vec4(fragPos, 1.0)).xy / scale)).z;
//
//    float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(fragDepth - sampleDepth));
//    occlusion += (sampleDepth >= fragDepth + BIAS ? 1.0 : 0.0) * rangeCheck;

    vec4 fragSample = vec4(fragPos, 1.0);
    fragSample = projection * fragSample;
    fragSample.xyz /= fragSample.w;
    fragSample.xyz = fragSample.xyz * 0.5 + 0.5;

    float occlusion = 0.0;
    for (int i = 0; i < KERNELS; ++i) {
        vec3 smpl = tbn * samples[i]; // From tangent to view-space
        smpl = fragPos + smpl * RADIUS;

        vec4 offset = vec4(smpl, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = vec3(view * texture(gPosition, offset.xy)).z;
        float fragDepth = vec3(view * texture(gPosition, fragSample.xy)).z;

        float rangeCheck = smoothstep(0.0, 1.0, RADIUS / abs(sampleDepth - fragDepth));
        occlusion += (sampleDepth <= fragDepth + BIAS ? 1.0 : 0.0) * rangeCheck;
    }

    outColor = occlusion / KERNELS;
}