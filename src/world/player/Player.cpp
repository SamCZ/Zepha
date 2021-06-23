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
	if (assert) assertField(Serializer().append(NetField::DIM).append(dim->getInd()).packet());
}

void Player::setPos(vec3 pos, bool assert) {
	Entity::setPos(pos);
	if (assert) assertField(Serializer().append(NetField::POS).append(pos).packet());
}

void Player::setVel(vec3 vel, bool assert) {
	Entity::setVel(vel);
	if (assert) assertField(Serializer().append(NetField::VEL).append(vel).packet());
}

f32 Player::getYaw() {
	return yaw;
}

void Player::setYaw(f32 yaw, bool assert) {
	this->yaw = yaw;
	if (assert) assertField(Serializer().append(NetField::LOOK_YAW).append(yaw).packet());
}

f32 Player::getPitch() {
	return pitch;
}

void Player::setPitch(f32 pitch, bool assert) {
	this->pitch = pitch;
	if (assert) assertField(Serializer().append(NetField::LOOK_PITCH).append(pitch).packet());
}

vec3 Player::getLookOffset() {
	return this->lookOffset;
}

void Player::setLookOffset(vec3 lookOffset, bool assert) {
	this->lookOffset = lookOffset;
	if (assert) assertField(Serializer().append(NetField::LOOK_OFF).append(lookOffset).packet());
}


bool Player::isFlying() {
	return flying;
}

void Player::setFlying(bool flying, bool assert) {
	this->flying = flying;
	if (assert) assertField(Serializer().append(NetField::FLYING).append(flying).packet());
}

string Player::getHandList() {
	return handList;
}

void Player::setHandList(const string& list, bool assert) {
	handList = list;
	if (assert) assertField(Serializer().append(NetField::HAND_INV).append(handList).packet());
}

string Player::getWieldList() {
	return wieldList;
}

void Player::setWieldList(const string& list, bool assert) {
	wieldList = list;
	if (assert) assertField(Serializer().append(NetField::WIELD_INV).append(wieldList).packet());
}

u16 Player::getWieldIndex() {
	return wieldIndex;
}

void Player::setWieldIndex(u16 index, bool assert) {
	wieldIndex = index;
	if (assert) assertField(Serializer().append(NetField::WIELD_INDEX).append(index).packet());
}