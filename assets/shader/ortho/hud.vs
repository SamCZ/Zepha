#version 330 core

layout (location = 0) in vec3  aPos;
layout (location = 1) in vec4  aColorData;
layout (location = 2) in vec3  aColorBlend;
layout (location = 3) in float aUseTex;
layout (location = 4) in vec3  aNormal;
layout (location = 5) in ivec4 aBoneIDs;
layout (location = 6) in vec4  aBoneWeights;

uniform mat4 model;
uniform mat4 ortho;

const int MAX_BONES = 100;
uniform mat4 uBones[MAX_BONES];

out vec4  colorData;
out vec3  colorBlend;
out float normalShading;
out float useTex;

void main() {
    mat4 boneTransform = uBones[aBoneIDs[0]] * aBoneWeights[0];
    boneTransform += uBones[aBoneIDs[1]] * aBoneWeights[1];
    boneTransform += uBones[aBoneIDs[2]] * aBoneWeights[2];
    boneTransform += uBones[aBoneIDs[3]] * aBoneWeights[3];

    float totalWeight = aBoneWeights[0] + aBoneWeights[1] + aBoneWeights[2] + aBoneWeights[3];

    boneTransform = (boneTransform * totalWeight) + (mat4(1.0) * (1 - totalWeight));

    if (aNormal == vec3(0, 0, 0)) normalShading = 1;
    else {
        vec3 normal = normalize(transpose(inverse(mat3(model))) * (boneTransform * vec4(normalize(aNormal), 0.0)).xyz);
        normalShading = 1 + (normal.x * -normal.z * 0.15) - (normal.y * 0.18);
    }

    gl_Position = ortho * model * boneTransform * vec4(aPos, 1);
    colorData = aColorData;
    colorBlend = aColorBlend;
    useTex = aUseTex;
}
