//
// Created by aurailus on 15/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

#ifndef GLPROJECT_MESHCHUNK_H
#define GLPROJECT_MESHCHUNK_H

#include "../../engine/Entity.h"
#include "../../../generic/helpers/TransPos.h"

class MeshChunk : Entity {
public:
    MeshChunk() = default;
    void build(std::vector<float> *vertices, std::vector<unsigned int> *indices);
    void setPos(glm::vec3 pos);
public:
    using Entity::setMesh;
    using Entity::draw;

    using Entity::getPos;
    using Entity::getModelMatrix;
};


#endif //GLPROJECT_MESHCHUNK_H

#pragma clang diagnostic pop