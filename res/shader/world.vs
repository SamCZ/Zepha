
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out float fogAlpha;
out float shading;

out vec2 fragTex;

void main() {
    vec4 mvPos = view * model * vec4(pos, 1.0);
    gl_Position = projection * mvPos;

    shading = (0.8 + abs(nor.x) * 0.15) + (nor.y * 0.15) + 0.2;
    fogAlpha = distance(vec3(0, 0, 0), vec3(mvPos));
	fragTex = tex;
}
