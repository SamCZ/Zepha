//
// Created by aurailus on 01/12/18.
//

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include "BlockModel.h"
#include <vector>
#include <gtx/normal.hpp>

const int CHUNK_SIZE = 16;

class MeshGenerator {
public:
    MeshGenerator();
    void build(int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], BlockModel* model,
               std::vector<float> &vertices, std::vector<unsigned int> &indices);

    ~MeshGenerator();
private:
    unsigned int indOffset;

    void addFaces(int x, int y, int z, vector<float>* vertices, vector<unsigned int>* indices, vector<MeshPart*>* meshParts);

    void cleanup();
};

#endif //GLPROJECT_MESHGENERATOR_H
