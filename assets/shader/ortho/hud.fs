#version 420 core

in float useTex;
in vec4 colorData;

out vec4 fragColor;

layout (binding = 0) uniform sampler2D tex;

void main() {
    if (useTex > 0.5) {
        fragColor = texture(tex, colorData.xy);
    }
    else {
        fragColor = colorData;
    }
}
