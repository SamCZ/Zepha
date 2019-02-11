#version 330

in float useTex;
in vec4 color;
in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    if (useTex > 0.5) {
        vec2 texCoord = fragTex;
        fragColor = texture(tex, texCoord) * color;
    }
    else {
        fragColor = color;
    }
}
