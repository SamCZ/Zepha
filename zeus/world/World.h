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

#include "BlockChunk.h"
#include "MeshChunk.h"
#include "../blocks/BlockAtlas.h"

class World {
public:
    struct vec3cmp {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    World();
    explicit World(BlockAtlas* atlas);

    void genChunk(glm::vec3 pos);
    void newChunk(glm::vec3 pos, BlockChunk* c);

    void update();

    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp>* getMeshChunks();

    ~World() = default;

    struct MeshThreadData {
        MeshThreadData(glm::vec3 pos, BlockChunk* chunk, BlockAtlas* atlas);

        std::thread* thread;

        glm::vec3 pos;
        BlockChunk* chunk;
        BlockAtlas* atlas;

        bool done;

        std::vector<float>* vertices;
        std::vector<unsigned int>* indices;

        ~MeshThreadData();
    };

    struct ChunkThreadData {
        ChunkThreadData(glm::vec3 pos, BlockAtlas* atlas);

        std::thread* thread;

        glm::vec3 pos;
        BlockAtlas* atlas;

        bool done;

        BlockChunk* chunk;

        ~ChunkThreadData();
    };
private:
    std::unordered_map<glm::vec3, BlockChunk*, vec3cmp> blockChunks;
    std::unordered_map<glm::vec3, MeshChunk*, vec3cmp> meshChunks;

    void handleChunkGenQueue();
    void handleMeshGenQueue();

    //TODO: Replace this BiQueueThreadArray model with a BiQueueThreadPool model (it's in the name)

    //Chunk Gen BiQueue Variables
    const int MAX_CHUNK_GEN_THREADS = 32;
    std::unordered_set<glm::vec3, vec3cmp> chunkGenQueue;
    std::vector<ChunkThreadData*> chunkGenThreads;

    //Mesh Gen BiQueue Variables
    const int MAX_MESH_GEN_THREADS = 32;
    std::unordered_set<glm::vec3, vec3cmp> meshGenQueue;
    std::vector<MeshThreadData*> meshGenThreads;

    BlockAtlas* blockAtlas;
};


#endif //GLPROJECT_WORLD_H
