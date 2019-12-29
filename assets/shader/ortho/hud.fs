#version 330 core

in vec4  colorData;
in vec3  colorBlend;
in float useTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    vec4 color;

    if (useTex > 0.5) {
        color = texture(tex, colorData.xy) * vec4(colorBlend, colorData.w);
    }
    else {
        color = colorData * vec4(colorBlend, 1);
    }

    if (color.w == 0) discard;
    fragColor = color;
}
