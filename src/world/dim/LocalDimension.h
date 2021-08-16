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
	LocalDimension(SubgamePtr game, LocalWorld& world, const string& identifier, u16 ind, sptr<MapGen> mapGen);
	
	void deactivate();
	
	/** Updates chunks and entities. */
	void update(f64 delta) override;
	
	/** Sets the chunk, and then queues it to be meshed. */
	void setChunk(sptr<Chunk> chunk) override;
	
	/** Sets the block, and queues the relevant chunks for remeshing. */
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
	
	void removeServerEntities(Deserializer& d);
	
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
	
	/** Queues a chunk to be meshed if its dirty and the adjacent chunks exist. */
	void meshChunk(const sptr<Chunk>& chunk, bool priority = false, bool updateAdjacents = true);
	
	/** Checks if a chunk exists and optionally meshes it if dirty. */
	bool adjacentExists(ivec3 pos, bool update, bool priority = false);
	
	sptr<MeshGenStream> meshGenStream;
	
	std::unordered_map<i64, ent_ref> entityRefs {};
	std::list<Api::Usertype::Entity> entities {};
	
	std::unordered_map<vec3, chunk_ref, Vec::vec3> renderRefs{};
	std::list<sptr<ChunkRenderElem>> renderElems{};
	
	u32 mapBlockScanY = 0;
	const static u32 MAPBLOCK_SCAN_Y_INTERVAL = 8;
	
	const ivec2 retainMapBlockRange = { 4, 4 };
	i64 entityInd = -1;
};

