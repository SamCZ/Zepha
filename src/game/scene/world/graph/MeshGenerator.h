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
#include "../../../../def/item/BlockModelVertex.h"
#include "../../../../world/chunk/BlockChunk.h"
#include "../../../../def/LocalDefs.h"
#include "../../../../def/item/BlockDef.h"
#include "../../../../def/gen/NoiseSample.h"
#include "../../../../util/Vec.h"
#include "../../../../util/Timer.h"
#include "../MeshDetails.h"

class MeshGenerator {
public:
    MeshGenerator(MeshDetails* meshDetails, LocalDefs& defs,
                  std::shared_ptr<BlockChunk> chunk, std::array<std::shared_ptr<BlockChunk>, 6> adjacent,
                  std::array<NoiseSample, 3>& blockOffsets);
private:
    BlockDef& getBlockAt(const glm::ivec3 &pos);
    void addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts, const glm::vec3& tint);

    unsigned int indOffset = 0;

    LocalDefs& defs;
    LocalDefinitionAtlas& atlas;
    MeshDetails* meshDetails;

    std::shared_ptr<BlockChunk> chunk;
    std::array<std::shared_ptr<BlockChunk>, 6> adjacent;

};

#pragma clang diagnostic pop