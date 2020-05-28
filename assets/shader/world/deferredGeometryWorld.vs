#version 330 core

#define TAU 6.28318530718
#define MAX_BLOCKLIGHT 31
#define MAX_SUNLIGHT 15

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec2  aTexCoords;
layout (location = 2) in vec3  aBlend;
layout (location = 3) in vec2  aBlendMaskCoords;
layout (location = 4) in float aNormal;
layout (location = 5) in vec4  aLight;
layout (location = 6) in float aShaderMod;
layout (location = 7) in vec3  aModValues;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform sampler2D swayTex;
uniform float time;

out vec2 texCoords;
out vec2 blendMaskCoords;
out vec3 blend;
out vec3 fragPos;
out vec3 normal;
out vec3 light;

vec3 unpackFloat(float src) {
    return vec3(fract(src) * 2.0f - 1.0f, fract(src * 256.f) * 2.0f - 1.0f, fract(src * 65536.f) * 2.0f - 1.0f);
}

vec4 rotateX(vec4 vertex, vec4 offset, float radians) {
    vertex -= offset;
    mat2 m = mat2(cos(radians), -sin(radians), sin(radians), cos(radians));
    return vec4(m * vertex.zy, vertex.xw).zyxw + offset;
}

vec4 rotateY(vec4 vertex, vec4 offset, float radians) {
    vertex -= offset;
    mat2 m = mat2(cos(radians), -sin(radians), sin(radians), cos(radians));
    return vec4(m * vertex.xz, vertex.yw).xzyw + offset;
}

vec4 rotateZ(vec4 vertex, vec4 offset, float radians) {
    vertex -= offset;
    mat2 m = mat2(cos(radians), -sin(radians), sin(radians), cos(radians));
    return vec4(m * vertex.xy, vertex.zw).xyzw + offset;
}

void main() {
    vec4 pos = vec4(aPos, 1);
    vec4 nml = vec4(unpackFloat(aNormal), 1);

    switch (int(aShaderMod)) {
        default: break;
        case 1: { // Rotate X
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8) + 0.5, 1);
            pos = rotateX(pos, origin, time * TAU * aModValues.y);
            nml = rotateX(nml, vec4(0), time * TAU * aModValues.y);
            break;
        }
        case 2: { // Rotate Y
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8) + 0.5, 1);
            pos = rotateY(pos, origin, time * TAU * aModValues.y);
            nml = rotateY(nml, vec4(0), time * TAU * aModValues.y);
            break;
        }
        case 3: { // Rotate Z
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8) + 0.5, 1);
            pos = rotateZ(pos, origin, time * TAU * aModValues.y);
            nml = rotateZ(nml, vec4(0), time * TAU * aModValues.y);
            break;
        }
        case 4: { //Sway Grounded
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8), 1);
            vec3 bsp = vec3(pos - origin);
            vec3 worldPos = (model * pos).xyz;
            if (bsp.x*bsp.y*bsp.z != 0 && bsp.x*bsp.y*bsp.z != 1) {
                vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f) / 16.f).xyz - .5f) * vec3(aModValues.y, aModValues.y / 2, aModValues.y);
                pos += vec4(sway, 0);
            }
            break;
        }
        case 5: { //Sway Full Block
            vec3 worldPos = (model * pos).xyz;
            vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f)  / 16.f).xyz - .5f) * vec3(aModValues.y, aModValues.y / 2, aModValues.y);
            pos += vec4(sway, 0);
            break;
        }
    }


    float sunlightIntensity = 1;
//    float sunlightIntensity = aLight.w * clamp(sin(time / 2.5) + 0.25, 0, 1) / MAX_SUNLIGHT;
    vec3 blockLightColor = (aLight.xyz / MAX_BLOCKLIGHT) * vec3(1 + sunlightIntensity / 4);
    vec3 sunlightColor = clamp(sunlightIntensity * 1.25 * vec3(1, 1, 1) * (aLight.w / 15.0), 0, 1);
    vec3 resultantLight = vec3(max(sunlightColor.x, blockLightColor.x), max(sunlightColor.y, blockLightColor.y), max(sunlightColor.z, blockLightColor.z));

    vec4 worldPos = model * pos;
//    worldPos.y -= pow(length(view * worldPos * 0.125) - 0.125, 2);
    worldPos.y -= pow(length(view * worldPos * 0.025) - 0, 2);

    fragPos = (view * worldPos).xyz;
    texCoords = aTexCoords;
    blendMaskCoords = aBlendMaskCoords;
    blend = aBlend;
    normal = nml.xyz;
    light = resultantLight;
    gl_Position = projection * view * worldPos;
}
