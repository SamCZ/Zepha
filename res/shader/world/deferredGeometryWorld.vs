#version 330 core

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

vec4 rotateX(vec4 vertex, vec4 offset, float radians) {
    vertex -= offset;
    mat2 m = mat2(cos(radians), -sin(radians), sin(radians), cos(radians));
    return vec4(m * vertex.zy, vertex.xw).zyxw + offset;
}

void main() {
    vec4 pos = vec4(aPos, 1);
    vec4 nml = vec4(unpackFloat(aNormal), 1);

    switch (int(aShaderMod)) {
        default: break;
        case 1: { // Rotate X
            pos = rotateX(pos, vec4(aModValues + vec3(0.5), 0), time);
            nml = rotateX(nml, vec4(0), time);
        }
    }

    vec4 worldPos = model * pos;

    fragPos = worldPos.xyz;
    texCoords = aTexCoords;
    normal = nml.xyz;

    gl_Position = projection * view * worldPos;
}
