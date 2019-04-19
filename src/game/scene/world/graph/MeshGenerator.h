//
// Created by aurailus on 01/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include <gtx/normal.hpp>
#include <vector>
#include <cstdio>

#include "../../../../util/Timer.h"
#include "../../../../def/block/BlockDef.h"
#include "../../../../world/chunk/BlockChunk.h"
#include "../../../../def/block/BlockAtlas.h"
#include "../../../../util/Vec.h"
#include "../../../../def/block/graph/BlockModel.h"
#include "MeshVertex.h"

class MeshGenerator {
public:
    MeshGenerator();
    void build(const std::shared_ptr<BlockChunk> &chunk, BlockAtlas &atlas, std::vector<bool> &adjacents,
          std::vector<float> &vertices, std::vector<unsigned int> &indices);

    ~MeshGenerator();
private:
    unsigned int indOffset;

    void addFaces(glm::vec3 &offset, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart*> &meshParts);

    void cleanup();
};

#endif //GLPROJECT_MESHGENERATOR_H

#pragma clang diagnostic pop