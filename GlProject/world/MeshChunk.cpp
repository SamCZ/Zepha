//
// Created by aurailus on 15/12/18.
//

#include "MeshChunk.h"

MeshChunk::MeshChunk() {
    ready = false;
}

bool MeshChunk::isReady() {
    return ready;
}

void MeshChunk::build(std::vector<float> *vertices, std::vector<unsigned int> *indices) {
    auto *mesh = new Mesh();
    mesh->create(vertices, indices);
    this->create(mesh);
    ready = true;
}