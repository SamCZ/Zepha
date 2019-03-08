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

#include "WorldThreadDefs.h"
#include "../../../generic/helpers/PerlinNoise.h"
#include "../../../generic/helpers/ArrayTrans3D.h"
#include "../../../generic/blocks/BlockAtlas.h"
#include "../../../generic/blocks/BlockChunk.h"
#include "../../../generic/gen/MapGen.h"
#include "../../graphics/mesh/MeshChunk.h"

class LocalWorld {
public:
    //Hashing function for glm::vec3 in maps and lists
    struct vec3cmp {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    //Functions for the thread pool implementations
    static void chunkGenThread(ChunkThreadDef* threadDef);
    static void meshGenThread(MeshThreadDef* threadDef);

    explicit LocalWorld(BlockAtlas* atlas);

    void genNewChunk(glm::vec3 pos);
    void commitChunk(glm::vec3 pos, BlockChunk *c);
    void remeshChunk(glm::vec3 pos);

    void loadChunkPacket(Packet* p);

    void attemptMeshChunk(glm::vec3 pos);
    //This function also updates the chunk that is being checked's adjacent data, so maybe a rename is in order.
    bool getAdjacentExists(glm::vec3 pos, glm::vec3 myPos);
    std::vector<bool>* getAdjacentsCull(glm::vec3 pos);

    void update();

    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp>* getMeshChunks();

    BlockChunk* getChunk(glm::vec3 chunkPos);

    int getBlock(glm::vec3 pos);
    void setBlock(glm::vec3 pos, int block);

    bool solidAt(glm::vec3 pos);

    static glm::vec3 roundVec(glm::vec3 vec) {
        return glm::vec3(floor(vec.x), floor(vec.y), floor(vec.z));
    }

    static glm::vec3 chunkVec(glm::vec3 globalVec) {
        return glm::vec3(std::floor(globalVec.x / 16), std::floor(globalVec.y / 16), std::floor(globalVec.z / 16));
    }

    static glm::vec3 localVec(glm::vec3 globalVec) {
        glm::vec3 out;

        if (globalVec.x < 0)
            out.x = 15 + (((int)globalVec.x + 1) % CHUNK_SIZE);
        else
            out.x = ((int)globalVec.x) % CHUNK_SIZE;

        if (globalVec.y < 0)
            out.y = 15 + (((int)globalVec.y + 1) % CHUNK_SIZE);
        else
            out.y = ((int)globalVec.y) % CHUNK_SIZE;

        if (globalVec.z < 0)
            out.z = 15 + (((int)globalVec.z + 1) % CHUNK_SIZE);
        else
            out.z = ((int)globalVec.z) % CHUNK_SIZE;

        return out;
    }

    int lastGenUpdates = 0, lastMeshUpdates = 0;
private:
    //Global lists for storing blockChunks and meshChunks
    std::unordered_map<glm::vec3, BlockChunk*, vec3cmp> blockChunks;
    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp> meshChunks;

    void handleChunkGenQueue();
    void handleMeshGenQueue();

    const int GEN_THREADS = 8;
    const int GEN_QUEUE_SIZE = 8;
    const int GEN_FINISHED_SIZE = GEN_THREADS * GEN_QUEUE_SIZE;

    std::vector<glm::vec3> pendingGen;
    std::vector<ChunkThreadDef*> genThreads;
    std::vector<ChunkThreadData*> finishedGen;

    const int MESH_THREADS = 8;
    const int MESH_QUEUE_SIZE = 8;
    const int MESH_FINISHED_SIZE = GEN_THREADS * GEN_QUEUE_SIZE;

    std::vector<glm::vec3> pendingMesh;
    std::vector<MeshThreadDef*> meshThreads;
    std::vector<MeshThreadData*> finishedMesh;

    BlockAtlas* blockAtlas;
    MapGen* mapGen;
};

#endif //GLPROJECT_WORLD_H
