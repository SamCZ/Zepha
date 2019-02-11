#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

uniform mat4 model;
uniform mat4 ortho;

out float useTex;
out vec4 color;
out vec2 fragTex;

out int texMode;

void main() {
    gl_Position = ortho * model * vec4(pos, 1.0);

    if (nor.x != 0 || nor.y != 0 || nor.z != 0) {
        useTex = 0;
        color = vec4(nor, tex.y);
    }
    else {
        useTex = 1;
        color = vec4(1, 1, 1, 1);
	    fragTex = tex;
    }
}
