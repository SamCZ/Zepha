//
// Created by aurailus on 04/04/19.
//

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

class LocalDimension : public Dimension {
public:
	const static int MB_STORE_H = 6;
	const static int MB_STORE_V = 4;
	
	LocalDimension(SubgamePtr game, LocalWorld& world, const std::string& identifier, unsigned int ind,
		std::shared_ptr<MapGen> mapGen);
	
	void deactivate();
	
	void update(double delta) override;
	
	void setChunk(std::shared_ptr<Chunk> chunk) override;
	
	bool setBlock(glm::ivec3 pos, unsigned int block) override;
	
	virtual void blockPlace(const Target& target, PlayerPtr player) override;
	
	virtual void blockPlaceOrInteract(const Target& target, PlayerPtr player) override;
	
	virtual void blockInteract(const Target& target, PlayerPtr player) override;
	
	virtual double blockHit(const Target& target, PlayerPtr player) override;
	
	virtual void wieldItemUse(const Target& target, PlayerPtr player) override;
	
	void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
	
	void removeMeshChunk(const glm::ivec3& pos);
	
	virtual long long nextEntityInd() override;
	
	void addLocalEntity(Api::Usertype::Entity entity);
	
	void removeLocalEntity(Api::Usertype::Entity entity);
	
	void serverEntitiesInfo(Deserializer& d);
	
	void serverEntitiesRemoved(Deserializer& d);
	
	std::vector<Api::Usertype::Entity> getEntitiesInRadius(glm::vec3 pos, float radius);
	
	Api::Usertype::Entity& getEntityById(long long id);
	
	void renderChunks(Renderer& renderer);
	
	void renderEntities(Renderer& renderer);
	
	uint32_t getMeshChunksDrawn();
	uint32_t getMeshChunksCommitted();
	
	int lastMeshesDrawn = 0;
	int lastMeshesCommitted = 0;
	std::vector<PlayerEntity> playerEntities;
	
protected:
	std::unordered_set<glm::ivec3, Vec::ivec3> propogateAddNodes() override;
	
	std::unordered_set<glm::ivec3, Vec::ivec3> propogateRemoveNodes() override;
	
private:
	typedef std::list<Api::Usertype::Entity>::iterator ent_ref;
	typedef std::list<std::shared_ptr<ChunkRenderElem>>::iterator chunk_ref;
	
	void finishMeshes();
	
	void attemptMeshChunk(const std::shared_ptr<Chunk>& chunk, bool priority = false, bool updateAdjacents = true);
	
	bool getAdjacentExists(glm::vec3 pos, bool updateAdjacents);
	
	std::shared_ptr<MeshGenStream> meshGenStream;
	
	std::unordered_map<long long, ent_ref> entityRefs {};
	std::list<Api::Usertype::Entity> entities {};
	
	std::unordered_map<glm::vec3, chunk_ref, Vec::vec3> renderRefs{};
	std::list<std::shared_ptr<ChunkRenderElem>> renderElems{};
	
	long long entityInd = -1;
};

