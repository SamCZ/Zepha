#pragma once

#include "util/Types.h"
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
	
	virtual void setPos(vec3 pos, bool assert = false);
	
	virtual void setVel(vec3 vel, bool assert = false);
	
	virtual float getYaw();
	
	virtual void setYaw(f32 yaw, bool assert = false);
	
	virtual f32 getPitch();
	
	virtual void setPitch(f32 pitch, bool assert = false);
	
	virtual vec3 getLookOffset();
	
	virtual void setLookOffset(vec3 lookOffset, bool assert = false);
	
	virtual bool isFlying();
	
	virtual void setFlying(bool flying, bool assert = false);
	
	virtual string getHandList();
	
	virtual void setHandList(const string& list, bool assert = false);
	
	virtual string getWieldList();
	
	virtual void setWieldList(const string& list, bool assert = false);
	
	virtual u16 getWieldIndex();
	
	virtual void setWieldIndex(u16 index, bool assert = false);
	
	virtual InventoryPtr getInventory() = 0;
	
	virtual void handleAssertion(Deserializer& d) = 0;
	
protected:
	virtual void assertField(Packet packet) = 0;
	
	World& world;
	
	f32 yaw = 0;
	f32 pitch = 0;
	
	vec3 lookOffset{};
	
	bool flying = true;
	
	string handList = "";
	string wieldList = "";
	
	u16 handItem = DefinitionAtlas::AIR;
	u16 wieldItem = DefinitionAtlas::AIR;
	
	u16 wieldIndex = 0;
};
