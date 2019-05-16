//
// Created by aurailus on 08/04/19.
//

#ifndef ZEUS_WIREFRAMEGENERATOR_H
#define ZEUS_WIREFRAMEGENERATOR_H

#include <vec3.hpp>
#include "../../graph/Mesh.h"
#include "../Entity.h"

class WireframeEntity : public Entity {
public:
    WireframeEntity() = default;
    WireframeEntity(glm::vec3 posA, glm::vec3 posB, float width, glm::vec3 color = {0.25, 0.25, 0.25});

    void updateMesh(glm::vec3 posA, glm::vec3 posB, float width, glm::vec3 color = {0.25, 0.25, 0.25});
private:
    std::vector<Vertex> vertices {};
    std::vector<unsigned int> indices {};

    void buildMesh();
    void createBox(float x, float y, float z, float xSize, float ySize, float zSize);

    glm::vec3 color {};
    glm::vec3 a     {};
    glm::vec3 b     {};

    float width = 0.5;
    int indOffset = 0;
};

#endif //ZEUS_WIREFRAMEGENERATOR_H
