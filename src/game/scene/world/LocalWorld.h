//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_WORLD_H
#define GLPROJECT_WORLD_H

#include <iostream>
#include <unordered_set>
#include <bits/unordered_map.h>
#include <thread>
#include <mutex>
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../../def/block/client/LocalBlockAtlas.h"
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/gen/MapGen.h"
#include "../../../util/TransPos.h"
#include "../../../util/Vec.h"
#include "../../../world/Dimension.h"
#include "../../../def/LocalDefs.h"
#include "../../../world/block/PointedThing.h"
#include "../../../game/graph/drawable/DrawableGroup.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../entity/world/ParticleEntity.h"
#include "graph/MeshChunk.h"
#include "WorldInterpolationStream.h"
#include "MeshGenStream.h"

class ClientNetworkInterpreter;

class LocalWorld {
public:
    LocalWorld(LocalDefs& defs, glm::vec3* playerPos, ClientNetworkInterpreter* server);

    void init();
    void update(double delta);

    void finishMeshes();
    void queueMeshes();
    void finishChunks();
    void updateBlockDamages(double delta);

    void loadChunkPacket(Packet p);
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 chunkPos);
    void commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk>);
    void remeshChunk(glm::vec3 pos);

    void damageBlock(glm::vec3 pos, float amount);

    void attemptMeshChunk(glm::vec3 pos);
    bool getAdjacentExists(glm::vec3 pos, glm::vec3 myPos);

    int renderChunks(Renderer &render);
    void renderEntities(Renderer &renderer);
    int getMeshChunkCount();

    int getBlock(glm::vec3 pos);

    //Called by the Client
    void localSetBlock(glm::vec3 pos, int block);
    //Called form ClientNetworkInterpreter
    void setBlock(glm::vec3 pos, int block);

    bool solidAt(glm::vec3 pos);

    int lastGenUpdates = 0, lastMeshUpdates = 0;
private:
    LocalDefs& defs;
    glm::vec3* playerPos;
    glm::vec3 playerChunkPos {};

    ClientNetworkInterpreter* server = nullptr;

    WorldInterpolationStream* worldGenStream = nullptr;
    Dimension dimension;

    std::vector<BlockCrackEntity*> crackedBlocks;
    std::vector<ParticleEntity*> particles;

    MeshGenStream meshGenStream;
    std::vector<glm::vec3> pendingMesh;
};

#endif //GLPROJECT_WORLD_H
