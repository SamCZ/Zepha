#pragma once

#include <list>

#include "Dimension.h"

#include "lua/usertype/Entity.h"

class ServerWorld;
class ServerPlayer;
class ServerSubgame;

/**
 * A server representation of a dimension,
 * contains all of the blocks and entities.
 */
 
class ServerDimension : public Dimension {
public:
	ServerDimension(SubgamePtr game, ServerWorld& world, const std::string& identifier, unsigned int ind,
		std::shared_ptr<MapGen> mapGen);
	
	virtual void update(double delta) override;
	
	void setChunk(std::shared_ptr<Chunk> chunk) override;
	
	virtual void blockPlace(const Target& target, PlayerPtr player) override;
	
	virtual void blockPlaceOrInteract(const Target& target, PlayerPtr player) override;
	
	virtual void blockInteract(const Target& target, PlayerPtr player) override;
	
	virtual double blockHit(const Target& target, PlayerPtr player) override;
	
	virtual void wieldItemUse(const Target& target, PlayerPtr player) override;
	
	virtual i64 nextEntityInd() override;
	
	void addLuaEntity(Api::Usertype::Entity entity);
	
	void removeLuaEntity(Api::Usertype::Entity entity);
	
	std::list<Api::Usertype::Entity>& getLuaEntities();
	
	const std::list<i64>& getRemovedEntities() const;
	
	void clearRemovedEntities();
	
private:
	typedef std::list<Api::Usertype::Entity>::iterator luaent_ref;
	
	std::unordered_map<i64, luaent_ref> luaEntityRefs{};
	std::list<Api::Usertype::Entity> luaEntities{};
	std::list<i64> removedEntities{};
	
	const ivec2 discardRange = { 20, 20 };
	i64 entityInd = 1;
};

