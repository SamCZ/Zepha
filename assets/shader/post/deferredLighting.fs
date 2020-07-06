#version 330 core

#define NEAR_FOG vec3(0.40, 0.56, 0.72)
#define FAR_FOG vec3(0.58, 0.76, 0.94)

out vec4 outColor;

in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssaoSampler;

uniform vec3 camPosition;

void main() {
    vec3 fragPos = texture(gPosition, texCoords).rgb;
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 color  = texture(gColorSpec, texCoords).rgb;
    float ssao  = texture(ssaoSampler, texCoords).r;

    //Shade based on Normals
    float shading = (0.95 + abs(normal.x) * 0.1) + (normal.y * 0.15);
    color *= vec3(shading);
    color *= ssao;

    //Apply fog color based on distance from camera
//    float dist = distance(camPosition, fragPos);
//    float nearFog = min(max(dist - 200, 0) / 100, 1);
//    float farFog = min(max(dist - 250, 0) / 100, 1);
//
//    color = mix(mix(color, NEAR_FOG, nearFog), FAR_FOG, farFog);

    outColor = vec4(color, 1);
}