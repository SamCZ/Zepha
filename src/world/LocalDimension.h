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
#include "../lua/api/type/LuaEntity.h"
#include "../game/entity/world/PlayerEntity.h"
#include "Dimension.h"

class LocalDimension : public Dimension {
public:
    explicit LocalDimension(LocalDefs& defs);
    void update(double delta, glm::vec3 playerPos);

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::vec3 pos, unsigned int block) override;

    void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
    void removeMeshChunk(const glm::vec3& pos);

    void addLuaEntity(sptr<LuaEntity>& entity);
    void removeLuaEntity(sptr<LuaEntity>& entity);

    int renderChunks(Renderer &renderer);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    int lastMeshUpdates = 0;
    std::vector<PlayerEntity> playerEntities;
private:
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

    std::unordered_map<glm::vec3, chunk_ref, Vec::compareFunc> renderRefs {};
    std::list<std::shared_ptr<ChunkRenderElem>> renderElems {};
};

