#version 440 core

#define RADIUS 1.0
#define RANGE 0.50
#define BIAS 0.08

out float outColor;

in vec2 texCoords;

layout (binding = 0) uniform sampler2D gPosition;
layout (binding = 1) uniform sampler2D gNormal;
layout (binding = 2) uniform sampler2D texNoise;

uniform uint kernels;
uniform vec3 samples[64]; //Must be a constant, setting to 64 because it's unlikely to go higher

uniform mat4 projection;
uniform mat4 view;

uniform float sampleScale;

const vec2 noiseScale = vec2(1920.0/4.0, 1004.0/4.0);

void main() {
    vec3 fragPos   = vec3(vec4(texture(gPosition, texCoords * sampleScale).xyz, 1.0f));
    vec3 normal    = normalize(mat3(view) * texture(gNormal, texCoords * sampleScale).xyz);
    vec3 randomVec = normalize(texture(texNoise, texCoords * sampleScale * noiseScale).xyz);

    if (normal == vec3(0)) discard;

    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn       = mat3(tangent, bitangent, normal);

    vec4 fragSample = vec4(fragPos, 1.0);
    fragSample = projection * fragSample;
    fragSample.xyz /= fragSample.w;
    fragSample.xyz = fragSample.xyz * 0.5 + 0.5;

    float fragDepth = vec3(texture(gPosition, fragSample.xy)).z;

    float occlusion = 0.0;
    for (int i = 0; i < kernels; ++i) {
        vec3 smpl = tbn * samples[i]; // From tangent to view-space
        smpl = fragPos + smpl * RADIUS;

        vec4 offset = vec4(smpl, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = vec3(texture(gPosition, offset.xy)).z;

        float rangeCheck = smoothstep(0.0, 1.0, RANGE / abs(fragDepth - sampleDepth));
        occlusion += (sampleDepth >= fragDepth + BIAS ? 1.0 : 0.0) * rangeCheck;
    }

    outColor = 1.0 - (occlusion / kernels / 2);
}