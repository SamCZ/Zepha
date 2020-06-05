#version 330 core

#define TAU 6.28318530718

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

struct SHADER_MOD {
    float type;
    vec3 values;
};

struct VS_OUT {
    vec3 pos;
    vec3 normal;

    vec2 blendMaskCoords;
    vec2 texCoords;
    vec3 blend;
    vec3 light;

    SHADER_MOD mod;
};

in VS_OUT gs_in[3];

out vec3 fragPos;
out vec3 normal;

out vec2 blendMaskCoords;
out vec2 texCoords;
out vec3 blend;
out vec3 light;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform sampler2D swayTex;
uniform float time;

vec3 unpackFloat(float src) { return vec3(fract(src) * 2.0f - 1.0f, fract(src * 256.f) * 2.0f - 1.0f, fract(src * 65536.f) * 2.0f - 1.0f); }

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

void processVertex(VS_OUT v) {
    vec4 pos = vec4(v.pos, 1);
    vec4 nml = vec4(v.normal, 1);

//    switch (int(v.mod.type)) {
//        default: break;
//        case 1: { // Rotate X
//            vec4 origin = vec4(round(unpackFloat(v.mod.values.x) * 8 + 8) + 0.5, 1);
//            pos = rotateX(pos, origin, time * TAU * v.mod.values.y);
//            nml = rotateX(nml, vec4(0), time * TAU * v.mod.values.y);
//            break;
//        }
//        case 2: { // Rotate Y
//            vec4 origin = vec4(round(unpackFloat(v.mod.values.x) * 8 + 8) + 0.5, 1);
//            pos = rotateY(pos, origin, time * TAU * v.mod.values.y);
//            nml = rotateY(nml, vec4(0), time * TAU * v.mod.values.y);
//            break;
//        }
//        case 3: { // Rotate Z
//            vec4 origin = vec4(round(unpackFloat(v.mod.values.x) * 8 + 8) + 0.5, 1);
//            pos = rotateZ(pos, origin, time * TAU * v.mod.values.y);
//            nml = rotateZ(nml, vec4(0), time * TAU * v.mod.values.y);
//            break;
//        }
//        case 4: { //Sway Grounded
//            vec4 origin = vec4(round(unpackFloat(v.mod.values.x) * 8 + 8), 1);
//            vec3 bsp = vec3(pos - origin);
//            vec3 worldPos = (model * pos).xyz;
//            if (bsp.x*bsp.y*bsp.z != 0 && bsp.x*bsp.y*bsp.z != 1) {
//                vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f) / 16.f).xyz - .5f) * vec3(v.mod.values.y, v.mod.values.y / 2, v.mod.values.y);
//                pos += vec4(sway, 0);
//            }
//            break;
//        }
//        case 5: { //Sway Full Block
//            vec3 worldPos = (model * pos).xyz;
//            vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f)  / 16.f).xyz - .5f) * vec3(v.mod.values.y, v.mod.values.y / 2, v.mod.values.y);
//            pos += vec4(sway, 0);
//            break;
//        }
//    }

    vec4 worldPos = model * pos;
    // worldPos.y -= pow(length(view * worldPos * 0.025) - 0, 2);
//    worldPos.y += sin(time + (worldPos.x + worldPos.z) / 10) * clamp(length(view * worldPos * 0.1) - 1, 0, 2.5);

    gl_Position = projection * view * worldPos;
    fragPos = (view * worldPos).xyz;
    normal = nml.xyz;

    // Passthrough
    blendMaskCoords = v.blendMaskCoords;
    texCoords = v.texCoords;
    blend = v.blend;
    light = v.light;

    EmitVertex();
}

void main() {
    for (int i = 0; i < 3; i++) processVertex(gs_in[i]);
}