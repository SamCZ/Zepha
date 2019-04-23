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
#include <vec3.hpp>
#include <gtc/type_ptr.hpp>

#include "../../../def/block/BlockAtlas.h"
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/gen/MapGen.h"
#include "graph/MeshChunk.h"
#include "../../../util/TransPos.h"
#include "WorldInterpolationStream.h"
#include "MeshGenStream.h"
#include "../../../util/Vec.h"
#include "../../../world/Dimension.h"
#include "../../../def/GameDefs.h"

class LocalWorld {
public:
    explicit LocalWorld(GameDefs& defs, glm::vec3* playerPos);

    void update();

    void commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk>);
    void remeshChunk(glm::vec3 pos);

    void loadChunkPacket(Packet* p);

    void attemptMeshChunk(glm::vec3 pos);
    bool getAdjacentExists(glm::vec3 pos, glm::vec3 myPos);

//    std::unordered_map<glm::vec3, MeshChunk*, VecUtils::compareFunc>* getMeshChunks();
    int render(Renderer& render);
    int getMeshChunkCount();
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 chunkPos);

    int getBlock(glm::vec3 pos);
    void setBlock(glm::vec3 pos, int block);

    bool solidAt(glm::vec3 pos);

    int lastGenUpdates = 0, lastMeshUpdates = 0;
private:
    GameDefs& defs;

    WorldInterpolationStream worldGenStream;
    Dimension dimension;

    std::unordered_map<glm::vec3, MeshChunk*, VecUtils::compareFunc> meshChunks;
    MeshGenStream meshGenStream;
    std::vector<glm::vec3> pendingMesh;
};

#endif //GLPROJECT_WORLD_H
