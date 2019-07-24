//
// Created by aurailus on 01/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/normal.hpp>
#include <vector>
#include <cstdio>

#include "MeshVertex.h"
#include "../../../../util/Timer.h"
#include "../../../../def/block/client/LocalBlockDef.h"
#include "../../../../world/chunk/BlockChunk.h"
#include "../../../../def/block/client/LocalBlockAtlas.h"
#include "../../../../def/block/client/LocalBlockModel.h"
#include "../../../../util/Vec.h"
#include "../../../../def/gen/NoiseSample.h"
#include "../MeshDetails.h"

class MeshGenerator {
public:
    MeshGenerator(MeshDetails* meshDetails, LocalBlockAtlas& atlas,
                  std::shared_ptr<BlockChunk> chunk, std::array<std::shared_ptr<BlockChunk>, 6> adjacent,
                  std::array<NoiseSample, 3>& blockOffsets);
private:
    LocalBlockDef& getBlockAt(const glm::vec3 &pos);
    void addFaces(const glm::vec3 &offset, const vector<LocalMeshPart> &meshParts);

    unsigned int indOffset = 0;

    LocalBlockAtlas& atlas;
    MeshDetails* meshDetails;

    std::shared_ptr<BlockChunk> chunk;
    std::array<std::shared_ptr<BlockChunk>, 6> adjacent;

};

#endif //GLPROJECT_MESHGENERATOR_H

#pragma clang diagnostic pop