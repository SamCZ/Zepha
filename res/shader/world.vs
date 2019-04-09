#version 330

layout (location = 0) in vec3  pos;
layout (location = 1) in float useTex;
layout (location = 2) in vec4  tex;
layout (location = 3) in vec3  nor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out float vertDist;
out float shading;

out float fragUseTex;
out vec4 color;
out vec2 fragTex;

void main() {
    vec4 mvPos = view * model * vec4(pos, 1.0);
    gl_Position = projection * mvPos;

    if (useTex > 0.5) {
        fragUseTex = 1;
        fragTex = vec2(tex.x, tex.y);
        color = vec4(1, 1, 1, 1);
    }
    else {
        fragUseTex = 0;
        fragTex = vec2(0, 0);
        color = tex;
    }

    shading = (0.8 + abs(nor.x) * 0.15) + (nor.y * 0.15) + 0.2;
    vertDist = distance(vec3(0, 0, 0), vec3(mvPos));
}
