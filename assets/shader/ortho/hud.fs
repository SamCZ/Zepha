#version 420 core

in vec4  colorData;
in vec3  colorBlend;
in float useTex;

out vec4 fragColor;

layout (binding = 0) uniform sampler2D tex;

void main() {
    if (useTex > 0.5)
        fragColor = texture(tex, colorData.xy) * vec4(colorBlend, 1);
    else
        fragColor = colorData * vec4(colorBlend, 1);
}
