//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_ENTITY_H
#define GLPROJECT_ENTITY_H

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

#include "Mesh.h"

class Entity {
public:
    Entity();

    void create(Mesh* mesh);
    void draw();
    void cleanup();

    void setPosition(glm::vec3 position);
    glm::vec3* getPosition();

    //void setRotation(glm::vec3 rotation);
    //glm::vec3* getRotation();

    void setScale(float scale);
    void setScale(glm::vec3 scale);
    glm::vec3* getScale();

    glm::mat4 getModelMatrix();

    ~Entity();
private:
    Mesh* mesh;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};


#endif //GLPROJECT_ENTITY_H
