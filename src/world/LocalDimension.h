//
// Created by aurailus on 04/04/19.
//

#pragma once

#include <unordered_map>
#include <glm/vec3.hpp>

#include "Dimension.h"
#include "region/Region.h"
#include "../util/Vec.h"
#include "../game/scene/world/MeshGenStream.h"
#include "../game/scene/world/graph/MeshChunk.h"
#include "../lua/api/type/LocalLuaEntity.h"
#include "../lua/api/type/ServerLocalLuaEntity.h"
#include "../game/entity/world/PlayerEntity.h"

class LocalDimension : public Dimension {
public:
    explicit LocalDimension(LocalDefs& defs);
    void update(double delta, glm::vec3 playerPos);

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
    void removeMeshChunk(const glm::ivec3& pos);

    void addLocalEntity(std::shared_ptr<LocalLuaEntity>& entity);
    void removeLocalEntity(std::shared_ptr<LocalLuaEntity>& entity);

    void handleServerEntity(const Packet& p);

    int renderChunks(Renderer &renderer);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    int lastMeshUpdates = 0;
    std::vector<PlayerEntity> playerEntities;
private:
    typedef std::list<sptr<ChunkRenderElem>>::iterator chunk_ref;
    typedef std::list<sptr<LocalLuaEntity>>::iterator local_ent_ref;
    typedef std::list<sptr<ServerLocalLuaEntity>>::iterator server_ent_ref;

    void finishMeshes();
    void queueMeshes();

    void attemptMeshChunk(const sptr<BlockChunk>& chunk, bool updateAdjacents = true);
    bool getAdjacentExists(glm::vec3 pos, bool updateAdjacents);

    LocalDefs& defs;

    uptr<MeshGenStream> meshGenStream = nullptr;
    std::vector<glm::vec3> pendingMesh {};

    std::unordered_map<uint, local_ent_ref> localEntityRefs {};
    std::list<std::shared_ptr<LocalLuaEntity>> localEntities {};

    std::unordered_map<uint, server_ent_ref> serverEntityRefs {};
    std::list<std::shared_ptr<ServerLocalLuaEntity>> serverEntities {};

    std::unordered_map<glm::vec3, chunk_ref, Vec::vec3> renderRefs {};
    std::list<std::shared_ptr<ChunkRenderElem>> renderElems {};
};

