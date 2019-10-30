#version 330 core

in vec4  colorData;
in vec3  colorBlend;
in float useTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    if (useTex > 0.5)
        fragColor = texture(tex, colorData.xy) * vec4(colorBlend, colorData.w);
    else
        fragColor = colorData * vec4(colorBlend, 1);
}
