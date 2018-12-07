#version 330

in vec4 color;
in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    vec2 texCoord = fragTex;
//    texCoord.x *= 0.03125;
//    texCoord.y *= 0.03125;
    fragColor = texture(tex, texCoord) * color;
}