#version 330 core

#define NEAR_FOG vec3(0.40, 0.56, 0.72)
#define FAR_FOG vec3(0.58, 0.76, 0.94)

out vec4 outColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;

void main() {
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal  = texture(gNormal, texCoords).rgb;
    vec3 color   = texture(gColorSpec, texCoords).rgb;

    float shading = (0.8 + abs(normal.x) * 0.15) + (normal.y * 0.15) + 0.2;
    float dist = distance(vec3(0, 0, 0), vec3(fragPos));

    float nearFog = min(max(dist - 200, 0) / 100, 1);
    float farFog = min(max(dist - 250, 0) / 100, 1);

    vec3 fragColor = color * vec3(shading);
    vec3 shadedColor = mix(mix(vec3(fragColor), NEAR_FOG, nearFog), FAR_FOG, farFog);

    outColor = vec4(shadedColor, 1);
}