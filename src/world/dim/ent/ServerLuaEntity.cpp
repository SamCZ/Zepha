#include <iostream>

#include "ServerLuaEntity.h"

#include "util/Log.h"
#include "world/dim/Dimension.h"
#include "util/net/Serializer.h"

void ServerLuaEntity::setDim(DimensionPtr newDim) {
	Entity::setDim(newDim);
	dirtyFields.emplace(NetField::DIM);
}

void ServerLuaEntity::setPos(glm::vec3 newPos) {
	Entity::setPos(newPos);
	dirtyFields.emplace(NetField::POS);
}

void ServerLuaEntity::setVel(glm::vec3 newVel) {
	Entity::setVel(newVel);
	dirtyFields.emplace(NetField::VEL);
}

void ServerLuaEntity::setRot(glm::vec3 newRot) {
	Entity::setRot(newRot);
	dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setScale(float newScale) {
	Entity::setScale(newScale);
	dirtyFields.emplace(NetField::SCALE);
}

void ServerLuaEntity::setScale(glm::vec3 newScale) {
	Entity::setScale(newScale);
	dirtyFields.emplace(NetField::SCALE);
}

void ServerLuaEntity::setRotateX(float newRotateX) {
	Entity::setRotateX(newRotateX);
	dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setRotateY(float newRotateY) {
	Entity::setRotateY(newRotateY);
	dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setRotateZ(float newRotateZ) {
	Entity::setRotateZ(newRotateZ);
	dirtyFields.emplace(NetField::ROT);
}

void ServerLuaEntity::setVisualOffset(glm::vec3 newVisualOff) {
	Entity::setVisualOffset(newVisualOff);
	dirtyFields.emplace(NetField::VISUAL_OFF);
}

void ServerLuaEntity::setCollisionBox(const SelectionBox& newCollisionBox) {
	Entity::setCollisionBox(newCollisionBox);
	dirtyFields.emplace(NetField::COLLISION_BOX);
}

void ServerLuaEntity::setAppearance(const std::string& mode, const std::string& argA, const std::string& argB) {
	dMode = mode;
	dArgA = argA;
	dArgB = argB;
	dirtyFields.emplace(NetField::DISPLAY);
}


void ServerLuaEntity::dirtyField(NetField field) {
	dirtyFields.emplace(field);
}

std::string ServerLuaEntity::serialize() {
	if (dirtyFields.empty()) return {};
	if (dirtyFields.count(NetField::ALL)) {
		dirtyFields.clear();
		dirtyFields.insert(NetField::ALL);
	}
	
	Serializer s;
	s.append<i64>(id);
	
	while (!dirtyFields.empty()) {
		const auto field = *dirtyFields.begin();
		dirtyFields.erase(field);
		
		switch (field) {
		default: {
			std::cout << Log::err << "Entity tried to serialize unhandled NetField, Type "
				<< static_cast<int>(field) << "." << Log::endl;
			break;
		}
		
		case NetField::ALL:
		
		case NetField::POS: {
			s.append(NetField::POS).append(pos);
			if (field != NetField::ALL) break;
		}
		
		case NetField::VEL: {
			s.append(NetField::VEL).append(vel);
			if (field != NetField::ALL) break;
		}
		
		case NetField::ROT: {
			s.append(NetField::ROT).append(rot);
			if (field != NetField::ALL) break;
		}
		
		case NetField::SCALE: {
			s.append(NetField::SCALE).append(scale);
			if (field != NetField::ALL) break;
		}
		
		case NetField::VISUAL_OFF: {
			s.append(NetField::VISUAL_OFF).append(visualOff);
			if (field != NetField::ALL) break;
		}
		
		case NetField::DISPLAY: {
			s.append(NetField::DISPLAY).append(dMode).append(dArgA).append(dArgB);
			if (field != NetField::ALL) break;
		}
		
		case NetField::ANIM_STATE: {
			s.append(NetField::ANIM_STATE).append<bool>(animation.isPlaying());
			if (field != NetField::ALL) break;
		}
		
		case NetField::ANIM_RANGE: {
			s.append(NetField::ANIM_RANGE).append(animation.getBounds()).append<bool>(animation.isLooping());
			if (field != NetField::ALL) break;
		}
		
		case NetField::DIM: {
			s.append(NetField::DIM).append(dim->getInd());
			if (field != NetField::ALL) break;
		}
		
		case NetField::COLLISION_BOX: {
			s.append(NetField::COLLISION_BOX).append<bool>(collisionBox.has_value());
			if (collisionBox) s.append(collisionBox->a).append(collisionBox->b);
			if (field != NetField::ALL) break;
		}
		
		case NetField::COLLIDES: {
			s.append(NetField::COLLIDES).append(collides);
			if (field != NetField::ALL) break;
		}
		
		case NetField::GRAVITY: {
			s.append(NetField::GRAVITY).append(gravity);
			if (field != NetField::ALL) break;
		}
		}
	}
	
	return s.data;
}