//
// Created by aurailus on 2020-07-28.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "util/CovariantPtr.h"
#include "world/dim/ent/Entity.h"
#include "game/atlas/DefinitionAtlas.h"

class World;

class Packet;

class Subgame;

class Inventory;

class Dimension;

class Deserializer;

class InventoryList;

class Player : public virtual Entity {
public:
	Player(SubgamePtr game, World& world, DimensionPtr dim, u32 id = 0) :
		Entity(game, dim),
		world(world), lookOffset(0, 1.65, 0) {
		collisionBox = {{ -0.3, 0, -0.3 }, { 0.3, 1.8, 0.3 }};
		this->id = id;
	}
	
	virtual void setDim(DimensionPtr dim, bool assert = false);
	
	virtual void setPos(glm::vec3 pos, bool assert = false);
	
	virtual void setVel(glm::vec3 vel, bool assert = false);
	
	virtual float getYaw();
	
	virtual void setYaw(float yaw, bool assert = false);
	
	virtual float getPitch();
	
	virtual void setPitch(float pitch, bool assert = false);
	
	virtual glm::vec3 getLookOffset();
	
	virtual void setLookOffset(glm::vec3 lookOffset, bool assert = false);
	
	virtual bool isFlying();
	
	virtual void setFlying(bool flying, bool assert = false);
	
	virtual std::string getHandList();
	
	virtual void setHandList(const std::string& list, bool assert = false);
	
	virtual std::string getWieldList();
	
	virtual void setWieldList(const std::string& list, bool assert = false);
	
	virtual unsigned short getWieldIndex();
	
	virtual void setWieldIndex(unsigned short index, bool assert = false);
	
	virtual InventoryPtr getInventory() = 0;
	
	virtual void handleAssertion(Deserializer& d) = 0;
	
protected:
	virtual void assertField(Packet packet) = 0;
	
	World& world;
	
	f32 yaw = 0;
	f32 pitch = 0;
	
	vec3 lookOffset{};
	
	bool flying = false;
	
	string handList = "";
	string wieldList = "";
	
	u16 handItem = DefinitionAtlas::AIR;
	u16 wieldItem = DefinitionAtlas::AIR;
	
	u16 wieldIndex = 0;
};
