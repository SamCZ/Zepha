#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

uniform mat4 model;
uniform mat4 ortho;

out vec4 color;
out vec2 fragTex;

void main() {
    gl_Position = ortho * model * vec4(pos, 1.0);

    color = vec4(1, 1, 1, 1);
	fragTex = tex;
}
