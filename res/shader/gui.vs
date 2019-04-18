#version 420

layout (location = 0) in vec3  pos;
layout (location = 1) in float useTex;
layout (location = 2) in vec4  tex;
layout (location = 3) in vec3  nor;

uniform mat4 model;
uniform mat4 ortho;

out float fragUseTex;
out vec4 color;
out vec2 fragTex;

out int texMode;

void main() {
    gl_Position = ortho * model * vec4(pos, 1.0);

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
}
