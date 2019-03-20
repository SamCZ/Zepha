
#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 nor;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec4 color;
out vec2 fragTex;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);

    vec4 myColor = vec4(1, 1, 1, 0) * (0.8 + abs(nor.x) * 0.15);
    myColor += nor.y * 0.15;
    myColor += 0.2;
    myColor.a = 1;

    float factor = min(gl_Position.length / 320, 1);

    color = myColor;
	fragTex = tex;
}
