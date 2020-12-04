//
// Created by aurailus on 2020-07-30.
//

#pragma once

#include <glm/vec3.hpp>

#include "AnimationState.h"
#include "util/CovariantPtr.h"
#include "game/def/mesh/SelectionBox.h"

class Entity {
public:
	Entity() = default;
	
	Entity(SubgamePtr game, DimensionPtr dim) : game(game), dim(dim) {};
	
	virtual long long getId() const;
	
	virtual void setId(long long newId);
	
	virtual glm::vec3 getPos() const;
	
	virtual void setPos(glm::vec3 position);
	
	virtual glm::vec3 getVel() const;
	
	virtual void setVel(glm::vec3 velocity);
	
	virtual glm::vec3 getRot() const;
	
	virtual void setRot(glm::vec3 newRot);
	
	virtual glm::vec3 getVisualOffset() const;
	
	virtual void setVisualOffset(glm::vec3 vs);
	
	virtual SelectionBox getCollisionBox() const;
	
	virtual void setCollisionBox(const SelectionBox& box);
	
	virtual float getRotateX() const;
	
	virtual void setRotateX(float rotation);
	
	virtual float getRotateY() const;
	
	virtual void setRotateY(float rotation);
	
	virtual float getRotateZ() const;
	
	virtual void setRotateZ(float rotation);
	
	virtual glm::vec3 getScale() const;
	
	virtual void setScale(float newScale);
	
	virtual void setScale(glm::vec3 newScale);
	
	virtual DimensionPtr getDim() const;
	
	virtual void setDim(DimensionPtr newDim);
	
	SubgamePtr getGame() const;
	
	virtual void update(double delta);
	
	AnimationState animation {};
	
protected:
	SubgamePtr game = nullptr;
	DimensionPtr dim = nullptr;
	
	long long id = 0;
	
	glm::vec3 pos {};
	glm::vec3 vel {};
	glm::vec3 rot {};
	glm::vec3 visualOff {};
	glm::vec3 scale { 1, 1, 1 };
	
	SelectionBox collision { {}, {} };
};
