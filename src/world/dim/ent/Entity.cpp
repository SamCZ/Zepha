//
// Created by aurailus on 2020-07-30.
//

#include "Entity.h"

long long Entity::getId() const {
	return id;
}

void Entity::setId(long long newId) {
	id = newId;
}

glm::vec3 Entity::getPos() const {
	return pos;
}

void Entity::setPos(glm::vec3 position) {
	pos = position;
}

glm::vec3 Entity::getVel() const {
	return vel;
}

void Entity::setVel(glm::vec3 velocity) {
	vel = velocity;
}

glm::vec3 Entity::getRot() const {
	return rot;
}

void Entity::setRot(glm::vec3 newRot) {
	rot = newRot;
}

glm::vec3 Entity::getVisualOffset() const {
	return visualOff;
}

void Entity::setVisualOffset(glm::vec3 vs) {
	visualOff = vs;
}

SelectionBox Entity::getCollisionBox() const {
	return collision;
}

void Entity::setCollisionBox(const SelectionBox& box) {
	collision = box;
}

float Entity::getRotateX() const {
	return rot.x;
}

void Entity::setRotateX(float rotation) {
	rot.x = rotation;
}

float Entity::getRotateY() const {
	return rot.y;
}

void Entity::setRotateY(float rotation) {
	rot.y = rotation;
}

float Entity::getRotateZ() const {
	return rot.z;
}

void Entity::setRotateZ(float rotation) {
	rot.z = rotation;
}

glm::vec3 Entity::getScale() const {
	return scale;
}

void Entity::setScale(float newScale) {
	scale = glm::vec3(newScale);
}

void Entity::setScale(glm::vec3 newScale) {
	scale = newScale;
}

DimensionPtr Entity::getDim() const {
	return dim;
}

void Entity::setDim(DimensionPtr newDim) {
	dim = newDim;
}

SubgamePtr Entity::getGame() const {
	return game;
}

void Entity::update(double delta) {
	animation.update(delta);
}