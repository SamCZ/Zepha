#version 330 core

#define TWO_PI 6.28318530718f

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec2  aTexCoords;
layout (location = 2) in float aNormal;
layout (location = 3) in float aShaderMod;
layout (location = 4) in vec3  aModValues;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform float time;

out vec2  texCoords;
out vec3  fragPos;
out vec3  normal;

vec3 unpackFloat(float src) {
    return vec3(fract(src) * 2.0f - 1.0f, fract(src * 256f) * 2.0f - 1.0f, fract(src * 65536f) * 2.0f - 1.0f);
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
            pos = rotateX(pos, origin, time * TWO_PI * aModValues.y);
            nml = rotateX(nml, vec4(0), time * TWO_PI * aModValues.y);
            break;
        }
        case 2: { // Rotate Y
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8) + 0.5, 1);
            pos = rotateY(pos, origin, time * TWO_PI * aModValues.y);
            nml = rotateY(nml, vec4(0), time * TWO_PI * aModValues.y);
            break;
        }
        case 3: { // Rotate Z
            vec4 origin = vec4(round(unpackFloat(aModValues.x) * 8 + 8) + 0.5, 1);
            pos = rotateZ(pos, origin, time * TWO_PI * aModValues.y);
            nml = rotateZ(nml, vec4(0), time * TWO_PI * aModValues.y);
            break;
        }
    }

    vec4 worldPos = model * pos;

    fragPos = worldPos.xyz;
    texCoords = aTexCoords;
    normal = nml.xyz;

    gl_Position = projection * view * worldPos;
}
