//
// Created by aurailus on 25/11/18.
//

#include <iostream>
#include "Entity.h"

Entity::Entity() {
    position = glm::vec3(0, 0, 0);
    scale = glm::vec3(1, 1, 1);
    angle = 0;
}

void Entity::create(Mesh* myMesh) {
    this->mesh = myMesh;
}

void Entity::create(Mesh* myMesh, Texture* texture) {
    this->mesh = myMesh;
    this->texture = texture;
}

void Entity::setTexture(Texture *texture) {
    this->texture = texture;
};

Texture* Entity::getTexture() {
    return texture;
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

void Entity::setAngle(GLfloat angle) {
    this->angle = angle;
}

GLfloat* Entity::getAngle() {
    return &angle;
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
    model = glm::rotate(model, angle * (GLfloat)(3.14159265 / 180), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);

    return model;
}

Entity::~Entity() {
    cleanup();
}