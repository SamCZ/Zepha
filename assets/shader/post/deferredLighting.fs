#version 330 core

#define NEAR_FOG vec3(0.40, 0.56, 0.72)
#define FAR_FOG vec3(0.58, 0.76, 0.94)

out vec4 outColor;

in vec2 texCoords;

//uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColorSpec;
uniform sampler2D ssaoSampler;

uniform vec3 camPosition;

void main() {
    vec3 normal = texture(gNormal, texCoords).rgb;
    vec3 color  = texture(gColorSpec, texCoords).rgb;
    float ssao  = texture(ssaoSampler, texCoords).r;

    //Shade based on Normals
    float shading = (0.8 + abs(normal.x) * 0.15) + (normal.y * 0.15) + 0.2;
    color *= vec3(shading);

    vec3 lighting = color;

    //Apply Lighting
//    lighting *= 0.1;
//
//    float radius = 16;
//
//    float lightDist = length(camPosition - fragPos);
//    if (lightDist < radius) {
//        vec3 lightDir = normalize(camPosition - fragPos);
//        vec3 diffuse = max(dot(normal, lightDir) * 0.6 + 0.4, 0.0) * color * vec3(1, 1, 1);
//        diffuse *= 1 - min(lightDist / radius, 1);
//        lighting += diffuse;
//    }

    //Apply fog color based on distance from camera
//    float dist = distance(vec3(0, 0, 0), vec3(fragPos));
//    float nearFog = min(max(dist - 200, 0) / 100, 1);
//    float farFog = min(max(dist - 250, 0) / 100, 1);

//    color = mix(mix(vec3(lighting), NEAR_FOG, nearFog), FAR_FOG, farFog);
    color = lighting * ssao;

    outColor = vec4(color, 1);
}