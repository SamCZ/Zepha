//
// Created by aurailus on 15/12/18.
//

#include "MeshChunk.h"

void MeshChunk::create(std::vector<ChunkVertex> &vertices, std::vector<unsigned int> &indices) {
    this->mesh = std::make_unique<ChunkMesh>();
    mesh->create(vertices, indices);
}

void MeshChunk::draw(Renderer& renderer) {
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, pos * static_cast<float>(TransPos::CHUNK_SIZE));
    renderer.setModelMatrix(model);
    mesh->draw();
}

void MeshChunk::setPos(glm::vec3 pos) {
    this->pos = pos;
}

glm::vec3 MeshChunk::getPos() {
    return pos;
}

bool MeshChunk::updateChunkUse(glm::vec3 pos, bool used) {
    return used;
}