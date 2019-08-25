//
// Created by aurailus on 25/11/18.
//

#include "Entity.h"

Entity::Entity() = default;

Entity::Entity(EntityMesh* mesh) {
    setMesh(mesh);
}

void Entity::setMesh(EntityMesh* myMesh) {
    cleanup();
    this->mesh = myMesh;
}

void Entity::draw(Renderer& renderer) {
    if (visible) {
        auto mm = getModelMatrix();
        renderer.setModelMatrix(mm);
        mesh->draw();
    }
}

void Entity::setPos(glm::vec3 position) {
    this->position = position;
}

glm::vec3 Entity::getPos() {
    return position;
}

void Entity::setAngle(float angle) {
    this->angle = angle;
}

float Entity::getAngle() {
    return angle;
}

void Entity::setScale(float scale) {
    setScale(glm::vec3(scale, scale, scale));
}

void Entity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

glm::vec3 Entity::getScale() {
    return scale;
}

glm::mat4 Entity::getModelMatrix() {
    glm::mat4 model = glm::mat4(1.0);

    model = glm::translate(model, position);
    model = glm::rotate(model, angle * (GLfloat)(3.14159265 / 180), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, scale);

    return model;
}

void Entity::cleanup() {
    delete mesh;
    mesh = nullptr;
}

Entity::~Entity() {
    cleanup();
}