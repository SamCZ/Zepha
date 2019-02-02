
#version 330

in vec4 color;
in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    vec2 texCoord = fragTex;
    vec4 color = texture(tex, texCoord) * color;
    if (color.a > 0.8) fragColor = color;
    else discard;
}
