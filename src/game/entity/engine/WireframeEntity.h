//
// Created by aurailus on 08/04/19.
//

#pragma once

#include "../Entity.h"

#include "../../../def/item/SelectionBox.h"
#include "../../graph/meshtypes/EntityVertex.h"

class WireframeEntity : public Entity {
public:
    WireframeEntity() = default;
    WireframeEntity(const std::vector<SelectionBox>& boxes, float width, glm::vec3 color);

    void updateMesh(const std::vector<SelectionBox>& boxes, float width);
private:
    std::vector<EntityVertex> vertices {};
    std::vector<unsigned int> indices {};

    void buildMesh(const std::vector<SelectionBox>& boxes);
    void createBox(glm::vec3 a, glm::vec3 b, float x, float y, float z, float xSize, float ySize, float zSize);

    glm::vec3 color {};

    float width = 0.5;
    int indOffset = 0;
};
