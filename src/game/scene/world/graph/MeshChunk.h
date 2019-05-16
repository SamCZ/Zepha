//
// Created by aurailus on 15/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

#ifndef GLPROJECT_MESHCHUNK_H
#define GLPROJECT_MESHCHUNK_H

#include "../../../entity/Entity.h"
#include "../../../../util/TransPos.h"

class MeshChunk : Entity {
public:
    MeshChunk() = default;
    void build(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void setPos(glm::vec3 pos);
    glm::vec3 getPos();
public:
    using Entity::setMesh;
    using Entity::draw;

    using Entity::getModelMatrix;
};


#endif //GLPROJECT_MESHCHUNK_H

#pragma clang diagnostic pop