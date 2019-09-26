//
// Created by aurailus on 25/09/19.
//

#include "EntityGeometryShader.h"

EntityGeometryShader::EntityGeometryShader(glm::vec2 windowSize, float bufferScale) : Shader(),
    windowSize(windowSize),
    bufferScale(bufferScale) {}

void EntityGeometryShader::postCreate() {
    uniforms.proj   = get("projection");
    uniforms.model  = get("model");
    uniforms.view   = get("view");

    uniforms.bones = get("uBones");
}


