//
// Created by aurailus on 01/12/18.
//

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include "BlockModel.h"
#include <vector>
#include <gtx/normal.hpp>
#include <cstdio>
#include "../engine/Timer.h"
#include "../blocks/BlockAtlas.h"
#include "../blocks/BlockChunk.h"

const int CHUNK_SIZE = 16;

class MeshGenerator {
public:
    MeshGenerator();
    void build(BlockChunk* chunk, BlockAtlas* atlas, std::vector<bool>* adjacents,
               std::vector<float> &vertices, std::vector<unsigned int> &indices);

    ~MeshGenerator();
private:
    unsigned int indOffset;

    void addFaces(glm::vec3 &offset, vector<float>* vertices, vector<unsigned int>* indices, vector<MeshPart*>* meshParts);

    void cleanup();
};

#endif //GLPROJECT_MESHGENERATOR_H
