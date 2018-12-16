//
// Created by aurailus on 01/12/18.
//

#include "MeshGenerator.h"
#include "../engine/helpers/ArrayTrans3D.h"

MeshGenerator::MeshGenerator() {
    indOffset = 0;
}

bool outOfRange(glm::vec3 pos) {
    return (pos.x < 0 || pos.x > 15 || pos.y < 0 || pos.y > 15 || pos.z < 0 || pos.z > 15);
}

void MeshGenerator::build(BlockChunk* chunk, BlockAtlas* atlas,
        std::vector<float> &vertices, std::vector<unsigned int> &indices) {

    Timer t("Mesh Generation");

    vertices.reserve(300000);
    indices.reserve(50000);

    glm::vec3 off;
    glm::vec3 check;

    BlockModel* model = atlas->getBlockModel();

    for (int i = 0; i < 4096; i++) {
        if (chunk->getBlock(i) == 1) {
            ArrayTrans3D::indAssignVec(i, &off);

            check.x = off.x - 1; check.y = off.y; check.z = off.z;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->leftFaces);

            check.x = off.x + 1; check.y = off.y; check.z = off.z;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->rightFaces);

            check.x = off.x; check.y = off.y - 1; check.z = off.z;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->bottomFaces);

            check.x = off.x; check.y = off.y + 1; check.z = off.z;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->topFaces);

            check.x = off.x; check.y = off.y; check.z = off.z - 1;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->backFaces);

            check.x = off.x; check.y = off.y; check.z = off.z + 1;
            if (outOfRange(check) || chunk->getBlock(&check) == 0)
                addFaces(off, &vertices, &indices, &model->frontFaces);
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();

    t.elapsedMs();
}

void MeshGenerator::addFaces(glm::vec3 &offset, vector<float>* vertices, vector<unsigned int>* indices, vector<MeshPart*>* meshParts) {
    for (MeshPart *mp : *meshParts) {

        MeshVertexIter *mvIterator = mp->getVertexIterator();
        while (mvIterator->hasNext()) {
            Vertex *vertex = mvIterator->next();

            vertices->push_back(vertex->pos->x + offset.x);
            vertices->push_back(vertex->pos->y + offset.y);
            vertices->push_back(vertex->pos->z + offset.z);

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