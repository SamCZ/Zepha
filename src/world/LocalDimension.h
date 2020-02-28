//
// Created by aurailus on 04/04/19.
//

#pragma once

#include "Dimension.h"
#include "../lua/api/class/LocalLuaEntity.h"
#include "../lua/api/class/ServerLocalLuaEntity.h"
#include "../game/entity/engine/PlayerEntity.h"
#include "../game/scene/world/MeshGenStream.h"
#include "../game/scene/world/graph/ChunkRenderElem.h"

class MeshChunk;

class LocalDimension : public Dimension {
public:
    const static int MB_STORE_H = 6;
    const static int MB_STORE_V = 4;

    explicit LocalDimension(ClientGame& defs);
    void update(double delta, glm::vec3 playerPos);

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    void setMeshChunk(std::shared_ptr<MeshChunk> chunk);
    void removeMeshChunk(const glm::ivec3& pos);

    void addLocalEntity(std::shared_ptr<LocalLuaEntity>& entity);
    void removeLocalEntity(std::shared_ptr<LocalLuaEntity>& entity);

    void serverEntityInfo(const Packet& p);
    void serverEntityRemoved(unsigned int id);

    int renderChunks(Renderer &renderer);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    int lastMeshUpdates = 0;
    std::vector<PlayerEntity> playerEntities;
private:
    typedef std::list<std::shared_ptr<ChunkRenderElem>>::iterator chunk_ref;
    typedef std::list<std::shared_ptr<LocalLuaEntity>>::iterator local_ent_ref;
    typedef std::list<std::shared_ptr<ServerLocalLuaEntity>>::iterator server_ent_ref;

    void finishMeshes();
    void queueMeshes();

    void attemptMeshChunk(const std::shared_ptr<BlockChunk>& chunk, bool updateAdjacents = true);
    bool getAdjacentExists(glm::vec3 pos, bool updateAdjacents);

    ClientGame& defs;

    std::unique_ptr<MeshGenStream> meshGenStream = nullptr;
    std::vector<glm::vec3> pendingMesh {};

    std::unordered_map<unsigned int, local_ent_ref> localEntityRefs {};
    std::list<std::shared_ptr<LocalLuaEntity>> localEntities {};

    std::unordered_map<unsigned int, server_ent_ref> serverEntityRefs {};
    std::list<std::shared_ptr<ServerLocalLuaEntity>> serverEntities {};

    std::unordered_map<glm::vec3, chunk_ref, Vec::vec3> renderRefs {};
    std::list<std::shared_ptr<ChunkRenderElem>> renderElems {};
};

