
#version 330

//#define NEAR_FOG vec3(0.73, 0.86, 1.0)
#define NEAR_FOG vec3(0.40, 0.56, 0.72)
#define FAR_FOG vec3(0.58, 0.76, 0.94)

in float fogAlpha;
in float shading;

in vec2 fragTex;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
    vec2 texCoord = fragTex;
    vec4 color = texture(tex, texCoord) * shading;

    float near = min(max(fogAlpha - 200, 0) / 100, 1);
    float far = min(max(fogAlpha - 250, 0) / 100, 1);
    color = vec4(mix(mix(vec3(color), NEAR_FOG, near), FAR_FOG, far), color.a);

    if (color.a > 0.8) fragColor = color;
    else discard;
}
