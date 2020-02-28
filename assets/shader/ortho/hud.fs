#version 330 core

in vec4  colorData;
in vec3  colorBlend;
in float normalShading;
in float useTex;

out vec4 fragColor;

uniform sampler2D tex;
uniform vec4 uClipBounds;

void main() {
    // Overflow clipping
    if (uClipBounds.z != 0 && uClipBounds.w != 0
     &&(gl_FragCoord.x < uClipBounds.x || gl_FragCoord.y < uClipBounds.y
     || gl_FragCoord.x > uClipBounds.z || gl_FragCoord.y > uClipBounds.w)) discard;

    vec4 color = vec4(1, 1, 1, 1);

    if (useTex > 0.5) {
        vec4 spec = texture(tex, colorData.xy) * vec4(colorBlend, colorData.w);
        if (spec.a <= 0) discard;
        color = spec;
    }
    else {
        if (colorData.a <= 0) discard;
        color = colorData * vec4(colorBlend, 1);
    }

    color *= vec4(vec3(normalShading), 1);
    fragColor = color;
}
