#version 420

in float fragUseTex;
in vec4 color;
in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    if (fragUseTex > 0.5)
        fragColor = texture(tex, fragTex) * color;
    else
        fragColor = color;
}
