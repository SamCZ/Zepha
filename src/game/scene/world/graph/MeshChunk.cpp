//
// Created by aurailus on 15/12/18.
//

#include "MeshChunk.h"

void MeshChunk::build(std::vector<float> *vertices, std::vector<unsigned int> *indices) {
    auto mesh = new Mesh();
    mesh->create(vertices, indices);

    this->setMesh(mesh);
}

void MeshChunk::setPos(glm::vec3 pos) {
    Entity::setPos(pos * glm::vec3(TransPos::CHUNK_SIZE));
}

glm::vec3 MeshChunk::getPos() {
    return Entity::getPos() / glm::vec3(TransPos::CHUNK_SIZE);
}
