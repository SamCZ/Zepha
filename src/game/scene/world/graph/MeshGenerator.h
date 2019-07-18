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

class MeshGenerator {
public:
    MeshGenerator(std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices,
                  LocalBlockAtlas& atlas, const BlockChunk& chunk, const std::vector<bool>& adj,
                  std::array<NoiseSample, 3>& blockOffsets);
private:
    bool faceOcculudedAt(const glm::vec3 &pos, const std::vector<bool> &adj);
    void addFaces(const glm::vec3 &offset, const vector<LocalMeshPart> &meshParts);
    LocalBlockDef& getDef(int ind);
    LocalBlockDef& getDef(const glm::vec3 &pos);

    unsigned int indOffset = 0;
    const BlockChunk& chunk;
    LocalBlockAtlas& atlas;
    std::vector<ChunkVertex>& vertices;
    std::vector<unsigned int>& indices;

};

#endif //GLPROJECT_MESHGENERATOR_H

#pragma clang diagnostic pop