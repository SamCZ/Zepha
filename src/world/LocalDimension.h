//
// Created by aurailus on 04/04/19.
//

#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>
#include "region/Region.h"
#include "../util/Vec.h"
#include "../game/scene/world/graph/MeshChunk.h"
#include "../game/scene/world/MeshGenStream.h"
#include "../game/entity/world/LuaEntity.h"
#include "../game/entity/world/PlayerEntity.h"

class LocalDimension {
public:
    explicit LocalDimension(LocalDefs& defs);
    void update(double delta, glm::vec3 playerPos);

    void setChunk(std::shared_ptr<BlockChunk> chunk);
    void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
    void removeMeshChunk(const glm::vec3& pos);

    void addLuaEntity(sptr<LuaEntity>& entity);
    void removeLuaEntity(sptr<LuaEntity>& entity);

    void setBlock(glm::vec3 pos, unsigned int block);
    unsigned int getBlock(glm::vec3 pos);

    int renderChunks(Renderer &renderer);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);

    int lastMeshUpdates = 0;
    std::vector<PlayerEntity> playerEntities;
private:
    typedef std::unordered_map<glm::vec3, std::shared_ptr<BlockChunk>, VecUtils::compareFunc> block_chunk_map;
    typedef std::list<sptr<ChunkRenderElem>>::iterator chunk_ref;
    typedef std::list<sptr<LuaEntity>>::iterator luaent_ref;

    void finishMeshes();
    void queueMeshes();

    void attemptMeshChunk(const sptr<BlockChunk>& chunk, bool updateAdjacents = true);
    bool getAdjacentExists(glm::vec3 pos, bool updateAdjacents);

    uptr<MeshGenStream> meshGenStream = nullptr;
    std::vector<glm::vec3> pendingMesh {};

    std::unordered_map<uint, luaent_ref> luaEntityRefs {};
    std::list<std::shared_ptr<LuaEntity>> luaEntities {};

    std::unordered_map<glm::vec3, chunk_ref, VecUtils::compareFunc> renderRefs {};
    std::list<std::shared_ptr<ChunkRenderElem>> renderElems {};

    block_chunk_map blockChunks;
};

