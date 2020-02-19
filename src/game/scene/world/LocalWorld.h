//
// Created by aurailus on 14/12/18.
//

#pragma once

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "World.h"

#include "../../../def/LocalDefinitionAtlas.h"
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/gen/MapGen.h"
#include "../../../util/Space.h"
#include "../../../util/Vec.h"
#include "../../../world/LocalDimension.h"
#include "../../../def/ClientGame.h"
#include "../../../world/block/PointedThing.h"
#include "../../../game/graph/drawable/DrawableGroup.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../entity/world/ParticleEntity.h"
#include "../../../lua/api/type/LocalLuaEntity.h"
#include "graph/MeshChunk.h"
#include "WorldInterpolationStream.h"
#include "MeshGenStream.h"

class ClientNetworkInterpreter;

class LocalWorld : public World {
public:
    LocalWorld(ClientGame& defs, glm::vec3* playerPos, ClientNetworkInterpreter* server);

    void init();
    void update(double delta) override;

    void loadChunkPacket(std::unique_ptr<Packet> p);
    void commitChunk(std::shared_ptr<BlockChunk> chunk);

    unsigned int getBlock(glm::ivec3 pos) override;
    void setBlock(glm::ivec3 pos, unsigned int block) override;
    void localSetBlock(glm::ivec3 pos, unsigned int block);

    void damageBlock(glm::vec3 pos, float amount);

    unsigned short getBiome(glm::vec3 pos);
    std::shared_ptr<BlockChunk> getChunk(glm::ivec3 pos);

    int getMeshChunkCount();
    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);

    ClientGame& defs;
    LocalDimension dimension;

    int lastGenUpdates = 0;
    int lastMeshUpdates = 0;
private:
    void finishChunks();
    void updateBlockDamages(double delta);

    glm::vec3* playerPos;

    std::vector<BlockCrackEntity*> crackedBlocks;
    std::vector<ParticleEntity*> particles;

    ClientNetworkInterpreter* server = nullptr;
    WorldInterpolationStream* worldGenStream = nullptr;
};
