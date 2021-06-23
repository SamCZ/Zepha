#include "Entity.h"
#include "Collision.h"

i64 Entity::getId() const {
	return id;
}

void Entity::setId(i64 newId) {
	id = newId;
}

vec3 Entity::getPos() const {
	return pos;
}

void Entity::setPos(vec3 newPos) {
	pos = newPos;
}

vec3 Entity::getVel() const {
	return vel;
}

void Entity::setVel(vec3 newVel) {
	vel = newVel;
}

vec3 Entity::getRot() const {
	return rot;
}

void Entity::setRot(vec3 newRot) {
	rot = newRot;
}

f32 Entity::getRotateX() const {
	return rot.x;
}

void Entity::setRotateX(f32 newRotateX) {
	rot.x = newRotateX;
}

f32 Entity::getRotateY() const {
	return rot.y;
}

void Entity::setRotateY(f32 newRotateY) {
	rot.y = newRotateY;
}

f32 Entity::getRotateZ() const {
	return rot.z;
}

void Entity::setRotateZ(f32 newRotateZ) {
	rot.z = newRotateZ;
}

vec3 Entity::getScale() const {
	return scale;
}

void Entity::setScale(f32 newScale) {
	scale = glm::vec3(newScale);
}

void Entity::setScale(vec3 newScale) {
	scale = newScale;
}

vec3 Entity::getVisualOffset() const {
	return visualOff;
}

void Entity::setVisualOffset(vec3 newVisualOff) {
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

f32 Entity::getGravity() const {
	return gravity;
}

void Entity::setGravity(f32 newGravity) {
	gravity = newGravity;
}

SubgamePtr Entity::getGame() const {
	return game;
}

void Entity::update(f64 delta) {
	static constexpr f32 TERMINAL_VELOCITY = 150.f;
	
	animation.update(delta);
	
	if (vel.length() != 0) {
		if (collides) {
			vec3 newPos, newVel;
			std::tie(newPos, newVel) = Collision::applyVel(
				game, dim, *collisionBox, pos, vel, delta);
			setPos(newPos);
			setVel(newVel);
		}
		else setPos(getPos() + vel * static_cast<f32>(delta));
	}
	
	if (gravity) {
		if (!Collision::isOnGround(game, dim, *collisionBox, pos, vel))
			setVel({ vel.x, std::fmax(vel.y - (gravity * delta), -TERMINAL_VELOCITY), vel.z });
		else if (vel.y < 0) setVel({ vel.x, 0, vel.z });
	}
}
