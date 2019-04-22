
#version 330

//#define NEAR_FOG vec3(0.73, 0.86, 1.0)
#define NEAR_FOG vec3(0.40, 0.56, 0.72)
#define FAR_FOG vec3(0.58, 0.76, 0.94)

in float vertDist;
in float shading;

in float fragUseTex;
in vec4 color;
in vec2 fragTex;

out vec4 outColor;

uniform sampler2D tex;

void main() {

    //Get Color
    vec4 fragColor;

    if (fragUseTex > 0.5) {
        vec2 texCoord = fragTex;
        fragColor = texture(tex, texCoord) * vec4(vec3(shading), 1);
    }
    else {
        fragColor = color * vec4(vec3(shading), 1);
    }

    //Apply Fog
    float near = min(max(vertDist - 200, 0) / 100, 1);
    float far = min(max(vertDist - 250, 0) / 100, 1);

    fragColor = vec4(mix(mix(vec3(fragColor), NEAR_FOG, near), FAR_FOG, far), fragColor.a);

    if (fragColor.a > 0.10) outColor = fragColor;
    else discard;
}
