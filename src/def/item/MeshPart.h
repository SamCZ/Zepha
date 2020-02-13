//
// Created by aurailus on 02/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <glm/vec4.hpp>
#include <glm/gtx/normal.hpp>

#include "ShaderMod.h"
#include "../texture/TextureAtlas.h"
#include "BlockModelVertex.h"

struct MeshPart {
    MeshPart(const std::vector<BlockModelVertex>& vertices, const std::vector<unsigned int>& indices,
             std::shared_ptr<AtlasRef> texture, unsigned int blendInd, std::shared_ptr<AtlasRef> blendMask);

    std::vector<BlockModelVertex> vertices;
    std::vector<unsigned int> indices;

    std::shared_ptr<AtlasRef> texture;
    unsigned int blendInd = 0;
    std::shared_ptr<AtlasRef> blendMask;

    ShaderMod shaderMod = ShaderMod::NONE;
    float modValue = 0;
};


#pragma clang diagnostic pop