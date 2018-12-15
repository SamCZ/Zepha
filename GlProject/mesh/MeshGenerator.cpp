//
// Created by aurailus on 01/12/18.
//

#include <cstdio>
#include "MeshGenerator.h"
#include "../engine/Timer.h"

MeshGenerator::MeshGenerator() {
    indOffset = 0;
}

bool outOfRange(glm::vec3 pos) {
    return (pos.x < 0 || pos.x > 15 || pos.y < 0 || pos.y > 15 || pos.z < 0 || pos.z > 15);
}

void MeshGenerator::build(int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], BlockModel* model,
                          std::vector<float> &vertices, std::vector<unsigned int> &indices) {

    Timer t("Mesh Generation");

    vertices.reserve(300000);
    indices.reserve(50000);

    glm::vec3 off;

    for (int i = 0; i < 4096; i++) {
        int idx = i;
        int z = idx / (CHUNK_SIZE * CHUNK_SIZE);
        idx -= (z * CHUNK_SIZE * CHUNK_SIZE);
        int y = idx / CHUNK_SIZE;
        int x = idx % CHUNK_SIZE;

        if (blocks[z][y][x] == 1) {

            if (outOfRange(glm::vec3(x - 1, y, z)) || blocks[z][y][x - 1] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->leftFaces);

            if (outOfRange(glm::vec3(x + 1, y, z)) || blocks[z][y][x + 1] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->rightFaces);

            if (outOfRange(glm::vec3(x, y - 1, z)) || blocks[z][y - 1][x] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->bottomFaces);

            if (outOfRange(glm::vec3(x, y + 1, z)) || blocks[z][y + 1][x] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->topFaces);

            if (outOfRange(glm::vec3(x, y, z - 1)) || blocks[z - 1][y][x] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->backFaces);

            if (outOfRange(glm::vec3(x, y, z + 1)) || blocks[z + 1][y][x] == 0)
                addFaces(x, y, z, &vertices, &indices, &model->frontFaces);
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    t.elapsedMs();
}

void MeshGenerator::addFaces(int x, int y, int z, vector<float>* vertices, vector<unsigned int>* indices, vector<MeshPart*>* meshParts) {
    for (MeshPart *mp : *meshParts) {

        MeshVertexIter *mvIterator = mp->getVertexIterator();
        while (mvIterator->hasNext()) {
            Vertex *vertex = mvIterator->next();

            vertices->push_back(vertex->pos->x + x);
            vertices->push_back(vertex->pos->y + y);
            vertices->push_back(vertex->pos->z + z);

            vertices->push_back(vertex->tex->x);
            vertices->push_back(vertex->tex->y);

            vertices->push_back(vertex->nml->x);
            vertices->push_back(vertex->nml->y);
            vertices->push_back(vertex->nml->z);
        }

        MeshIndexIter *miIterator = mp->getIndexIterator();
        while (miIterator->hasNext()) {
            unsigned int index = miIterator->next();
            indices->push_back(indOffset + index);
        }
        indOffset += mp->getVertexCount();
    }
}

void MeshGenerator::cleanup() {

}

MeshGenerator::~MeshGenerator() {
    cleanup();
}