//
// Created by aurailus on 02/12/18.
//

#include "Vertex.h"

Vertex::Vertex() = default;

Vertex::Vertex(glm::vec3* pos, glm::vec3* nml, glm::vec2* tex) {
    this->pos = pos;
    this->nml = nml;
    this->tex = tex;
}

Vertex::~Vertex() {
    delete pos;
    delete nml;
    delete tex;
}