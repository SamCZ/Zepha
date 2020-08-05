//
// Created by aurailus on 2020-07-30.
//

#include "Entity.h"

unsigned int Entity::getId() {
    return id;
}

void Entity::setId(unsigned int id) {
    this->id = id;
}

glm::vec3 Entity::getPos() {
    return pos;
}

void Entity::setPos(glm::vec3 position) {
    this->pos = position;
}

glm::vec3 Entity::getVel() {
    return vel;
}

void Entity::setVel(glm::vec3 velocity) {
    this->vel = velocity;
}

glm::vec3 Entity::getRot() {
    return rot;
}

void Entity::setRot(glm::vec3 rot) {
    this->rot = rot;
}

glm::vec3 Entity::getVisualOffset() {
    return visualOff;
}

void Entity::setVisualOffset(glm::vec3 vs) {
    this->visualOff = vs;
}

float Entity::getRotateX() {
    return rot.x;
}

void Entity::setRotateX(float rotation) {
    this->rot.x = rotation;
}

float Entity::getRotateY() {
    return rot.y;
}

void Entity::setRotateY(float rotation) {
    this->rot.y = rotation;
}

float Entity::getRotateZ() {
    return rot.z;
}

void Entity::setRotateZ(float rotation) {
    this->rot.z = rotation;
}

glm::vec3 Entity::getScale() {
    return scale;
}

void Entity::setScale(float scale) {
    this->scale = glm::vec3(scale);
}

void Entity::setScale(glm::vec3 scale) {
    this->scale = scale;
}

DimensionPtr Entity::getDim() {
    return dim;
}

void Entity::setDim(DimensionPtr dim) {
    this->dim = dim;
}

SubgamePtr Entity::getGame() {
    return game;
}
