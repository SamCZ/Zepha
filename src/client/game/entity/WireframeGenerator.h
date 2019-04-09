//
// Created by aurailus on 08/04/19.
//

#ifndef ZEUS_WIREFRAMEGENERATOR_H
#define ZEUS_WIREFRAMEGENERATOR_H


#include "../../engine/graphics/Mesh.h"
#include <vec3.hpp>

class WireframeGenerator {
public:
    WireframeGenerator(glm::vec3 a, glm::vec3 b, float width, glm::vec3 color = {0.1, 0.1, 0.1});
    Mesh* build();
private:
    std::vector<float> vertices {};
    std::vector<unsigned int> indices {};

    void createBox(float x, float y, float z, float xSize, float ySize, float zSize);

    glm::vec3 color {};
    glm::vec3 a {}, b {};
    float width;
    int indOffset = 0;
};


#endif //ZEUS_WIREFRAMEGENERATOR_H
