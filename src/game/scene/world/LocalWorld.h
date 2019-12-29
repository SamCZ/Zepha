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

#include "../../../def/LocalDefinitionAtlas.h"
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/gen/MapGen.h"
#include "../../../util/Space.h"
#include "../../../util/Vec.h"
#include "../../../world/LocalDimension.h"
#include "../../../def/LocalDefs.h"
#include "../../../world/block/PointedThing.h"
#include "../../../game/graph/drawable/DrawableGroup.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../entity/world/ParticleEntity.h"
#include "../../../lua/api/type/LocalLuaEntity.h"
#include "graph/MeshChunk.h"
#include "WorldInterpolationStream.h"
#include "MeshGenStream.h"

class ClientNetworkInterpreter;

class LocalWorld {
public:
    LocalWorld(LocalDefs& defs, glm::vec3* playerPos, ClientNetworkInterpreter* server);

    void init();
    void update(double delta);

    void loadChunkPacket(std::unique_ptr<Packet> p);
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 chunkPos);
    void commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk>);

    void damageBlock(glm::vec3 pos, float amount);

    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    unsigned int getBlock(glm::vec3 pos);
    unsigned short getBiome(glm::vec3 pos);

    //Called by the Client
    void localSetBlock(glm::vec3 pos, unsigned int block);
    //Called form ClientNetworkInterpreter
    void setBlock(glm::vec3 pos, unsigned int block);

    bool solidAt(glm::vec3 pos);

    int lastGenUpdates = 0;
    int lastMeshUpdates = 0;

    LocalDimension dimension;
    LocalDefs& defs;
private:
    void finishChunks();
    void updateBlockDamages(double delta);

    glm::vec3* playerPos;

    std::vector<BlockCrackEntity*> crackedBlocks;
    std::vector<ParticleEntity*> particles;

    ClientNetworkInterpreter* server = nullptr;
    WorldInterpolationStream* worldGenStream = nullptr;
};
