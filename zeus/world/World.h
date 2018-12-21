//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_WORLD_H
#define GLPROJECT_WORLD_H

#include <unordered_set>
#include <iostream>
#include <gtc/type_ptr.hpp>
#include <vec3.hpp>
#include <thread>
#include <bits/unordered_map.h>
#include <mutex>

#include "BlockChunk.h"
#include "MeshChunk.h"
#include "../blocks/BlockAtlas.h"

class World {
private:
    //Predeclare structs
    struct MeshThreadData;
    struct MeshThreadDef;
    struct ChunkThreadData;
    struct ChunkThreadDef;
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

    explicit World(BlockAtlas* atlas);

    void genNewChunk(glm::vec3 pos);
    void commitChunk(glm::vec3 pos, BlockChunk *c);

    void update();

    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp>* getMeshChunks();
private:
    //Global lists for storing blockChunks and meshChunks
    std::unordered_map<glm::vec3, BlockChunk*, vec3cmp> blockChunks;
    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp> meshChunks;

    void handleChunkGenQueue();
    void handleMeshGenQueue();

    const int GEN_THREADS = 8;
    const int GEN_QUEUE_SIZE = 16;
    const int GEN_FINISHED_SIZE = GEN_THREADS * GEN_QUEUE_SIZE;

    std::unordered_set<glm::vec3, vec3cmp> pendingGen;
    std::vector<ChunkThreadDef*> genThreads;
    std::vector<ChunkThreadData*> finishedGen;

    const int MESH_THREADS = 8;
    const int MESH_QUEUE_SIZE = 16;
    const int MESH_FINISHED_SIZE = GEN_THREADS * GEN_QUEUE_SIZE;

    std::unordered_set<glm::vec3, vec3cmp> pendingMesh;
    std::vector<MeshThreadDef*> meshThreads;
    std::vector<MeshThreadData*> finishedMesh;

    BlockAtlas* blockAtlas;

    //Structs for the thread pool implementations
    struct MeshThreadData {
        MeshThreadData(glm::vec3 pos, BlockChunk* chunk, BlockAtlas* atlas);

        glm::vec3 pos;
        BlockChunk* chunk;
        BlockAtlas* atlas;

        bool done;

        std::vector<float>* vertices;
        std::vector<unsigned int>* indices;

        ~MeshThreadData();
    };

    struct MeshThreadDef {
        MeshThreadDef();

        std::thread* thread;
        std::mutex lock;
        std::vector<MeshThreadData*> tasks;

        ~MeshThreadDef();
    };

    struct ChunkThreadData {
        ChunkThreadData(glm::vec3 pos, BlockAtlas* atlas);

        glm::vec3 pos;
        BlockAtlas* atlas;

        bool done;
        BlockChunk* chunk;
    };

    struct ChunkThreadDef {
        ChunkThreadDef();

        std::thread* thread;
        std::mutex lock;
        std::vector<ChunkThreadData*> tasks;

        ~ChunkThreadDef();
    };
};

#endif //GLPROJECT_WORLD_H
