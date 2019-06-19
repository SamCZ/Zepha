//
// Created by aurailus on 15/12/18.
//

#include "MeshChunk.h"

void MeshChunk::build(std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& indices) {
    auto mesh = new ChunkMesh();
    mesh->create(vertices, indices);

    cleanup();
    this->mesh = mesh;
}

void MeshChunk::draw(Renderer& renderer) {
    renderer.setModelMatrix(getModelMatrix());
    mesh->draw();
}

void MeshChunk::setPos(glm::vec3 pos) {
    this->pos = pos;
}

glm::vec3 MeshChunk::getPos() {
    return pos;
}

glm::mat4 MeshChunk::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);
    model = glm::translate(model, pos * 16.f);
    return model;
}

void MeshChunk::cleanup() {
    delete mesh;
    mesh = nullptr;
}

MeshChunk::~MeshChunk() {
    cleanup();
}