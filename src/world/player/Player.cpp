//
// Created by aurailus on 2020-07-28.
//

#include <iostream>

#include "Player.h"

#include "util/net/NetField.h"
#include "util/net/Serializer.h"
#include "world/dim/Dimension.h"

void Player::setDim(DimensionPtr dim, bool assert) {
	Entity::setDim(dim);
	if (assert) assertField(Serializer().appendEnum(NetField::DIM).append(dim->getInd()).packet());
}

void Player::setPos(glm::vec3 pos, bool assert) {
	Entity::setPos(pos);
	if (assert) assertField(Serializer().appendEnum(NetField::POS).append(pos).packet());
}

void Player::setVel(glm::vec3 vel, bool assert) {
	Entity::setVel(vel);
	if (assert) assertField(Serializer().appendEnum(NetField::VEL).append(vel).packet());
}

float Player::getYaw() {
	return yaw;
}

void Player::setYaw(float yaw, bool assert) {
	this->yaw = yaw;
	if (assert) assertField(Serializer().appendEnum(NetField::LOOK_YAW).append(yaw).packet());
}

float Player::getPitch() {
	return pitch;
}

void Player::setPitch(float pitch, bool assert) {
	this->pitch = pitch;
	if (assert) assertField(Serializer().appendEnum(NetField::LOOK_PITCH).append(pitch).packet());
}

glm::vec3 Player::getLookOffset() {
	return this->lookOffset;
}

void Player::setLookOffset(glm::vec3 lookOffset, bool assert) {
	this->lookOffset = lookOffset;
	if (assert) assertField(Serializer().appendEnum(NetField::LOOK_OFF).append(lookOffset).packet());
}


bool Player::isFlying() {
	return flying;
}

void Player::setFlying(bool flying, bool assert) {
	this->flying = flying;
	if (assert) assertField(Serializer().appendEnum(NetField::FLYING).append(flying).packet());
}

std::string Player::getHandList() {
	return handList;
}

void Player::setHandList(const std::string& list, bool assert) {
	handList = list;
	if (assert) assertField(Serializer().appendEnum(NetField::HAND_INV).append(handList).packet());
}

std::string Player::getWieldList() {
	return wieldList;
}

void Player::setWieldList(const std::string& list, bool assert) {
	wieldList = list;
	if (assert) assertField(Serializer().appendEnum(NetField::WIELD_INV).append(wieldList).packet());
}

unsigned short Player::getWieldIndex() {
	return wieldIndex;
}

void Player::setWieldIndex(unsigned short index, bool assert) {
	wieldIndex = index;
	if (assert) assertField(Serializer().appendEnum(NetField::WIELD_INDEX).append(index).packet());
}