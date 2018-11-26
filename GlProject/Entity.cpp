//
// Created by aurailus on 25/11/18.
//

#include <iostream>
#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0, 0, 0);
    rotation = glm::vec3(0, 0, 0);
    scale    = glm::vec3(1, 1, 1);
}

void Entity::create(Mesh* myMesh) {
    this->mesh = myMesh;
}

void Entity::draw() {
    mesh->draw();
}

void Entity::cleanup() {
    this->mesh->cleanup();
}

void Entity::setPosition(glm::vec3 position) {
    this->position = position;
}

glm::vec3* Entity::getPosition() {
    return &position;
}

void Entity::setScale(float scale) {
    setScale(glm::vec3(scale, scale, scale));
}

void Entity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec3* Entity::getScale() {
    return &scale;
}

glm::mat4 Entity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, position);
//  model = glm::rotate(model, triAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);

    return model;
}

Entity::~Entity() {
    cleanup();
};