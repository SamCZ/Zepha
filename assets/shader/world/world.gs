#version 330 core

#define TAU 6.28318530718

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 pos;
    vec3 normal;

    vec2 blendMaskCoords;
    vec2 texCoords;
    vec3 blend;
    vec3 light;

    float modType;
    vec3 modValues;
} gs_in[3];

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

void main() {
    for (int i = 0; i < 3; i++) {
        vec4 pos = vec4(gs_in[i].pos, 1);
        vec4 nml = vec4(gs_in[i].normal, 1);

        switch (int(gs_in[i].modType)) {
            default: break;
            case 1: { // Rotate X
                vec4 origin = vec4(round(unpackFloat(gs_in[i].modValues.x) * 8 + 8) + 0.5, 1);
                pos = rotateX(pos, origin, time * TAU * gs_in[i].modValues.y);
                nml = rotateX(nml, vec4(0), time * TAU * gs_in[i].modValues.y);
                break;
            }
            case 2: { // Rotate Y
                vec4 origin = vec4(round(unpackFloat(gs_in[i].modValues.x) * 8 + 8) + 0.5, 1);
                pos = rotateY(pos, origin, time * TAU * gs_in[i].modValues.y);
                nml = rotateY(nml, vec4(0), time * TAU * gs_in[i].modValues.y);
                break;
            }
            case 3: { // Rotate Z
                vec4 origin = vec4(round(unpackFloat(gs_in[i].modValues.x) * 8 + 8) + 0.5, 1);
                pos = rotateZ(pos, origin, time * TAU * gs_in[i].modValues.y);
                nml = rotateZ(nml, vec4(0), time * TAU * gs_in[i].modValues.y);
                break;
            }
            case 4: { //Sway Grounded
                vec4 origin = vec4(round(unpackFloat(gs_in[i].modValues.x) * 8 + 8), 1);
                vec3 bsp = vec3(pos - origin);
                vec3 worldPos = (model * pos).xyz;
                if (bsp.x * bsp.y * bsp.z != 0 && bsp.x * bsp.y * bsp.z != 1) {
                    vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f) / 16.f).xyz - .5f) * vec3(gs_in[i].modValues.y, gs_in[i].modValues.y / 2, gs_in[i].modValues.y);
                    pos += vec4(sway, 0);
                }
                break;
            }
            case 5: { //Sway Full Block
                vec3 worldPos = (model * pos).xyz;
                vec3 sway = (texture(swayTex, worldPos.xz * (worldPos.y / 16.f)  / 16.f).xyz - .5f) * vec3(gs_in[i].modValues.y, gs_in[i].modValues.y / 2, gs_in[i].modValues.y);
                pos += vec4(sway, 0);
                break;
            }
        }

        vec4 worldPos = model * pos;
//        worldPos.y -= pow(length(view * worldPos * 0.025) - 0, 2);

        gl_Position = projection * view * worldPos;
        fragPos = (view * worldPos).xyz;
        normal = nml.xyz;

        // Passthrough
        blendMaskCoords = gs_in[i].blendMaskCoords;
        texCoords = gs_in[i].texCoords;
        blend = gs_in[i].blend;
        light = gs_in[i].light;

        EmitVertex();
    }
}