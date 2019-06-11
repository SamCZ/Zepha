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
#include "../../../../def/block/client/LocalBlockDef.h"
#include "../../../../world/chunk/BlockChunk.h"
#include "../../../../def/block/client/LocalBlockAtlas.h"
#include "../../../../def/block/client/LocalBlockModel.h"
#include "../../../../util/Vec.h"
#include "MeshVertex.h"

class MeshGenerator {
public:
    MeshGenerator();
    void build(const std::shared_ptr<BlockChunk> &chunk, LocalBlockAtlas &atlas, std::vector<bool> &adjacents,
          std::vector<Vertex> &vertices, std::vector<unsigned int> &indices);
private:
    unsigned int indOffset;

    void addFaces(glm::vec3 &offset, vector<Vertex> &vertices, vector<unsigned int> &indices, vector<LocalMeshPart> &meshParts);
};

#endif //GLPROJECT_MESHGENERATOR_H

#pragma clang diagnostic pop