#include "Entity.h"
#include "Collision.h"

long long Entity::getId() const {
	return id;
}

void Entity::setId(long long newId) {
	id = newId;
}

glm::vec3 Entity::getPos() const {
	return pos;
}

void Entity::setPos(glm::vec3 newPos) {
	pos = newPos;
}

glm::vec3 Entity::getVel() const {
	return vel;
}

void Entity::setVel(glm::vec3 newVel) {
	vel = newVel;
}

glm::vec3 Entity::getRot() const {
	return rot;
}

void Entity::setRot(glm::vec3 newRot) {
	rot = newRot;
}

float Entity::getRotateX() const {
	return rot.x;
}

void Entity::setRotateX(float newRotateX) {
	rot.x = newRotateX;
}

float Entity::getRotateY() const {
	return rot.y;
}

void Entity::setRotateY(float newRotateY) {
	rot.y = newRotateY;
}

float Entity::getRotateZ() const {
	return rot.z;
}

void Entity::setRotateZ(float newRotateZ) {
	rot.z = newRotateZ;
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

glm::vec3 Entity::getVisualOffset() const {
	return visualOff;
}

void Entity::setVisualOffset(glm::vec3 newVisualOff) {
	visualOff = newVisualOff;
}

DimensionPtr Entity::getDim() const {
	return dim;
}

void Entity::setDim(DimensionPtr newDim) {
	dim = newDim;
}

std::optional<SelectionBox> Entity::getCollisionBox() const {
	return collisionBox;
}

void Entity::setCollisionBox(const SelectionBox& box) {
	collisionBox = box;
}

void Entity::removeCollisionBox() {
	collisionBox = std::nullopt;
}

bool Entity::getCollides() const {
	return collides;
}

void Entity::setCollides(bool newCollides) {
	collides = newCollides;
}

float Entity::getGravity() const {
	return gravity;
}

void Entity::setGravity(float newGravity) {
	gravity = newGravity;
}

SubgamePtr Entity::getGame() const {
	return game;
}

void Entity::update(double delta) {
	animation.update(delta);
	
	if (vel.length() != 0) {
		if (collides) {
			Collision::moveCollide(game, dim, *collisionBox, pos, vel,
				Collision::isOnGround(game, dim, *collisionBox, pos, vel) ? 0.6 : vel.y <= 0 ? 0.1 : 0);
		}
		else {
			setPos(getPos() + vel * static_cast<float>(delta));
		}
	}
}
