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

#include "../../../generic/helpers/ArrayTrans3D.h"
#include "../../../generic/blocks/BlockAtlas.h"
#include "../../../generic/blocks/BlockChunk.h"
#include "../../../generic/gen/MapGen.h"
#include "../../graphics/mesh/MeshChunk.h"
#include "../../../generic/helpers/Vec3Compare.h"
#include "../../../generic/helpers/ChunkVec.h"
#include "WorldInterpolationStream.h"
#include "MeshGenStream.h"

class LocalWorld {
public:
    explicit LocalWorld(BlockAtlas* atlas);

    void update();

    //INFO: Pushes chunk to the list unless one already exists, then tries to mesh it.
    void commitChunk(glm::vec3 pos, BlockChunk *c);

    //INFO: Attempts to regenerate a chunk mesh.
    void remeshChunk(glm::vec3 pos);

    //TODO: Remove this
    void loadChunkPacket(Packet* p);

    void attemptMeshChunk(glm::vec3 pos);
    //This function also updates the chunk that is being checked's adjacent data, so maybe a rename is in order.
    bool getAdjacentExists(glm::vec3 pos, glm::vec3 myPos);
    std::vector<bool>* getAdjacentsCull(glm::vec3 pos);

    std::unordered_map<glm::vec3, MeshChunk*, Vec3Compare::func>* getMeshChunks();

    BlockChunk* getChunk(glm::vec3 chunkPos);

    int getBlock(glm::vec3 pos);
    void setBlock(glm::vec3 pos, int block);

    bool solidAt(glm::vec3 pos);

    int lastGenUpdates = 0, lastMeshUpdates = 0;
private:
    BlockAtlas* blockAtlas;

    std::unordered_map<glm::vec3, BlockChunk*, Vec3Compare::func> blockChunks;
    WorldInterpolationStream worldGenStream;

    std::unordered_map<glm::vec3, MeshChunk*, Vec3Compare::func> meshChunks;
    MeshGenStream meshGenStream;
    std::vector<glm::vec3> pendingMesh;
};

#endif //GLPROJECT_WORLD_H
