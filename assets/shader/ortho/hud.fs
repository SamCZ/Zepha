#version 330 core

in vec4  colorData;
in vec3  colorBlend;
in float useTex;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec4 uClipBounds;

void main() {
    if (uClipBounds.z != 0 && uClipBounds.w != 0
     &&(gl_FragCoord.x < uClipBounds.x || gl_FragCoord.y < uClipBounds.y
     || gl_FragCoord.x > uClipBounds.z || gl_FragCoord.y > uClipBounds.w)) discard;

    if (useTex > 0.5) {
        vec4 spec = texture(tex, colorData.xy) * vec4(colorBlend, colorData.w);
        if (spec.a <= 0) discard;
        fragColor = spec;
    }
    else {
        if (colorData.a <= 0) discard;
        fragColor = colorData * vec4(colorBlend, 1);
    }
}
