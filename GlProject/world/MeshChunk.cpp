//
// Created by aurailus on 15/12/18.
//

#include "MeshChunk.h"

MeshChunk::MeshChunk() {
    ready = false;
}

MeshChunk::MeshChunk(BlockChunk *blockChunk) {
    this->blockChunk = blockChunk;
    ready = false;
}

bool MeshChunk::isReady() {
    return ready;
}

void MeshChunk::build(BlockAtlas* atlas) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    MeshGenerator mg;
    mg.build(blockChunk, atlas, vertices, indices);

    auto *mesh = new Mesh();
    mesh->create(&vertices, &indices);

    this->create(mesh);
}