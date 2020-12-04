//
// Created by aurailus on 2019-12-24.
//

#pragma once

#include <set>
#include <string>
#include <glm/vec3.hpp>

#include "world/dim/ent/LuaEntity.h"

#include "util/net/Packet.h"
#include "util/net/NetField.h"

class ServerLuaEntity : public LuaEntity {
	public:
	explicit ServerLuaEntity(SubgamePtr game, DimensionPtr dim, unsigned int id) :
		LuaEntity(game, dim), Entity(game, dim) {
		setId(id);
	};
	
	virtual void setDim(DimensionPtr dim) override;
	
	virtual void setPos(glm::vec3 position) override;
	
	virtual void setVel(glm::vec3 vel) override;
	
	virtual void setRot(glm::vec3 rot) override;
	
	virtual void setScale(float scale) override;
	
	virtual void setScale(glm::vec3 scale) override;
	
	virtual void setRotateX(float rotation) override;
	
	virtual void setRotateY(float rotation) override;
	
	virtual void setRotateZ(float rotation) override;
	
	virtual void setVisualOffset(glm::vec3 vs) override;
	
	virtual void setCollisionBox(const SelectionBox& box) override;
	
	void setAppearance(const std::string& dMode, const std::string& argA, const std::string& argB) override;
	
	void dirtyField(NetField field);
	
	std::string serialize();
	
	protected:
	std::string dMode = "";
	std::string dArgA = "";
	std::string dArgB = "";
	
	std::set<NetField> dirtyFields;
	bool fullSend = true;
};

