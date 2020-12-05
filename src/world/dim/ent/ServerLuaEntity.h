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
	
	virtual void setDim(DimensionPtr newDim) override;
	
	virtual void setPos(glm::vec3 newPos) override;
	
	virtual void setVel(glm::vec3 newVel) override;
	
	virtual void setRot(glm::vec3 newRot) override;
	
	virtual void setScale(float newScale) override;
	
	virtual void setScale(glm::vec3 newScale) override;
	
	virtual void setRotateX(float newRotateX) override;
	
	virtual void setRotateY(float newRotateY) override;
	
	virtual void setRotateZ(float newRotateX) override;
	
	virtual void setVisualOffset(glm::vec3 newVisualOff) override;
	
	virtual void setCollisionBox(const SelectionBox& newCollisionBox) override;
	
	void setAppearance(const std::string& mode, const std::string& argA, const std::string& argB) override;
	
	/**
	 * Marks a field as dirty, which will cause it to be included in the next serialize call.
	 * @param field - The NetField that should be marked as dirty.
	 */
	 
	void dirtyField(NetField field);
	
	/**
	 * Serializes dirty fields into a string, for transfer to the client.
	 * @returns an encoded string containing dirty entity data.
	 */
	
	std::string serialize();
	
protected:
	/** The display properties of the entity. */
	std::string dMode {}, dArgA {}, dArgB {};
	/** The set of dirty fields to be resent on the next serialize call. */
	std::set<NetField> dirtyFields { NetField::ALL };
};

