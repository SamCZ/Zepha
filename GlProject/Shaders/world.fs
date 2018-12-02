#version 330

in vec4 color;
in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
//	fragColor = color;
    fragColor = texture(tex, fragTex) * color;
}