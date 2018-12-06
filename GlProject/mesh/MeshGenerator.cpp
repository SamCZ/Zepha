//
// Created by aurailus on 01/12/18.
//

#include <cstdio>
#include "MeshGenerator.h"
#include "../engine/Timer.h"

MeshGenerator::MeshGenerator() {
    indOffset = 0;
}

void MeshGenerator::build(int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], BlockModel* model,
                          std::vector<float> &vertices, std::vector<unsigned int> &indices) {

    auto t = Timer("MeshGen");

    vertices.reserve(16384);
    indices.reserve(4096);

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                if (blocks[i][j][k] == 1) {

                    if (true) //Left Faces
                        addFaces(k, j, i, vertices, indices, model->leftFaces);
                    if (true) //Right Faces
                        addFaces(k, j, i, vertices, indices, model->rightFaces);

                    if (true) //Top Faces
                        addFaces(k, j, i, vertices, indices, model->topFaces);
                    if (true) //Bottom Faces
                        addFaces(k, j, i, vertices, indices, model->bottomFaces);

                    if (true) //Front Faces
                        addFaces(k, j, i, vertices, indices, model->frontFaces);
                    if (true) //Back Faces
                        addFaces(k, j, i, vertices, indices, model->backFaces);

                    if (true) //Non-culled Faces
                        addFaces(k, j, i, vertices, indices, model->noCulledFaces);
                }
            }
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    t.elapsed();
}

void MeshGenerator::addFaces(int x, int y, int z, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart*> meshParts) {
    for (MeshPart *mp : meshParts) {
        MeshVertexIter *mvIterator = mp->getVertexIterator();
        while (mvIterator->hasNext()) {
            Vertex *vertex = mvIterator->next();

            vertices.push_back(vertex->pos->x + x);
            vertices.push_back(vertex->pos->y + y);
            vertices.push_back(vertex->pos->z + z);

            vertices.push_back(vertex->tex->x);
            vertices.push_back(vertex->tex->y);

            vertices.push_back(vertex->nml->x);
            vertices.push_back(vertex->nml->y);
            vertices.push_back(vertex->nml->z);
        }

        MeshIndexIter *miIterator = mp->getIndexIterator();
        while (miIterator->hasNext()) {
            unsigned int index = miIterator->next();
            indices.push_back(indOffset + index);
        }
        indOffset += mp->getVertexCount();
    }
}

void MeshGenerator::cleanup() {

}

MeshGenerator::~MeshGenerator() {
    cleanup();
}