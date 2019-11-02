//
// Created by aurailus on 01/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/normal.hpp>
#include <vector>
#include <cstdio>

#include "../../../../def/item/MeshPart.h"
#include "../MeshDetails.h"
#include "../../../../def/item/BlockModelVertex.h"
#include "../../../../world/chunk/BlockChunk.h"
#include "../../../../def/LocalDefinitionAtlas.h"
#include "../../../../def/item/BlockDef.h"
#include "../../../../def/gen/NoiseSample.h"
#include "../../../../util/Vec.h"
#include "../../../../util/Timer.h"

class MeshGenerator {
public:
    MeshGenerator(MeshDetails* meshDetails, LocalDefinitionAtlas& atlas,
                  std::shared_ptr<BlockChunk> chunk, std::array<std::shared_ptr<BlockChunk>, 6> adjacent,
                  std::array<NoiseSample, 3>& blockOffsets);
private:
    BlockDef& getBlockAt(const glm::vec3 &pos);
    void addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts);

    unsigned int indOffset = 0;

    LocalDefinitionAtlas& atlas;
    MeshDetails* meshDetails;

    std::shared_ptr<BlockChunk> chunk;
    std::array<std::shared_ptr<BlockChunk>, 6> adjacent;

};

#pragma clang diagnostic pop