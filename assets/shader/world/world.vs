#version 330 core

#define MAX_BLOCKLIGHT 31
#define MAX_SUNLIGHT 15

layout (location = 0) in vec3  aPos;
layout (location = 4) in float aNormal;

layout (location = 3) in vec2  aBlendMaskCoords;
layout (location = 1) in vec2  aTexCoords;
layout (location = 2) in ivec3 aBlend;
layout (location = 5) in ivec4 aLight;

layout (location = 6) in int   aShaderMod;
layout (location = 7) in ivec3 aModValues;

out VS_OUT {
    vec3 pos;
    vec3 normal;

    vec2 blendMaskCoords;
    vec2 texCoords;
    vec3 blend;
    vec3 light;

    float modType;
    vec3 modValues;
} vs_out;

vec3 unpackFloat(float src) { return vec3(fract(src) * 2.0f - 1.0f, fract(src * 256.f) * 2.0f - 1.0f, fract(src * 65536.f) * 2.0f - 1.0f); }

void main() {
    float sunlightIntensity = 1;
    vec3 blockLightColor = (aLight.xyz / MAX_BLOCKLIGHT) * vec3(1 + sunlightIntensity / 4);
    vec3 sunlightColor = clamp(sunlightIntensity * 1.25 * vec3(1, 1, 1) * (aLight.w / 15.0), 0, 1);
    vec3 light = vec3(max(sunlightColor.x, blockLightColor.x), max(sunlightColor.y, blockLightColor.y), max(sunlightColor.z, blockLightColor.z));

    vs_out.pos = aPos;
    vs_out.normal = unpackFloat(aNormal);

    vs_out.blendMaskCoords = aBlendMaskCoords;
    vs_out.texCoords = aTexCoords;
    vs_out.blend = vec3(aBlend / 255.0);
    vs_out.light = light;

    vs_out.modType = aShaderMod;
    vs_out.modValues = aModValues;
}
