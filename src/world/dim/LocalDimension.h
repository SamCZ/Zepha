#pragma once

#include <list>

#include "Dimension.h"

#include "game/LocalSubgame.h"
#include "lua/usertype/Entity.h"
#include "client/entity/PlayerEntity.h"

class Renderer;
class MeshChunk;
class Deserializer;
class MeshGenStream;
class ChunkRenderElem;

/**
 * A local representation of a dimension,
 * contains all of the blocks and entities.
 */

class LocalDimension : public Dimension {
public:
	const static u8 MB_STORE_H = 6;
	const static u8 MB_STORE_V = 4;
	
	LocalDimension(SubgamePtr game, LocalWorld& world, const string& identifier, u16 ind, sptr<MapGen> mapGen);
	
	void deactivate();
	
	void update(f64 delta) override;
	
	void setChunk(sptr<Chunk> chunk) override;
	
	bool setBlock(ivec3 pos, u16 block) override;
	
	virtual void blockPlace(const Target& target, PlayerPtr player) override;
	
	virtual void blockPlaceOrInteract(const Target& target, PlayerPtr player) override;
	
	virtual void blockInteract(const Target& target, PlayerPtr player) override;
	
	virtual double blockHit(const Target& target, PlayerPtr player) override;
	
	virtual void wieldItemUse(const Target& target, PlayerPtr player) override;
	
	void setMeshChunk(sptr<MeshChunk> chunk);
	
	void removeMeshChunk(const ivec3& pos);
	
	virtual i64 nextEntityInd() override;
	
	void addLocalEntity(Api::Usertype::Entity entity);
	
	void removeLocalEntity(Api::Usertype::Entity entity);
	
	void serverEntitiesInfo(Deserializer& d);
	
	void serverEntitiesRemoved(Deserializer& d);
	
	std::vector<Api::Usertype::Entity> getEntitiesInRadius(vec3 pos, f32 radius);
	
	Api::Usertype::Entity& getEntityById(i64 id);
	
	void renderChunks(Renderer& renderer);
	
	void renderEntities(Renderer& renderer);
	
	u32 getMeshChunksDrawn();
	u32 getMeshChunksCommitted();
	
	u32 lastMeshesDrawn = 0;
	u32 lastMeshesCommitted = 0;
	vec<PlayerEntity> playerEntities;
	
protected:
	std::unordered_set<ivec3, Vec::ivec3> propogateAddNodes() override;
	
	std::unordered_set<ivec3, Vec::ivec3> propogateRemoveNodes() override;
	
private:
	typedef std::list<Api::Usertype::Entity>::iterator ent_ref;
	typedef std::list<std::shared_ptr<ChunkRenderElem>>::iterator chunk_ref;
	
	void finishMeshes();
	
	void attemptMeshChunk(const sptr<Chunk>& chunk, bool priority = false, bool updateAdjacents = true);
	
	bool getAdjacentExists(vec3 pos, bool updateAdjacents);
	
	sptr<MeshGenStream> meshGenStream;
	
	std::unordered_map<i64, ent_ref> entityRefs {};
	std::list<Api::Usertype::Entity> entities {};
	
	std::unordered_map<vec3, chunk_ref, Vec::vec3> renderRefs{};
	std::list<sptr<ChunkRenderElem>> renderElems{};
	
	i64 entityInd = -1;
};

