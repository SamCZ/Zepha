//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_WORLD_H
#define GLPROJECT_WORLD_H

#include <map>
#include <unordered_set>
#include <iostream>
#include <gtc/type_ptr.hpp>
#include <vec3.hpp>
#include <thread>

#include "BlockChunk.h"
#include "MeshChunk.h"
#include "../blocks/BlockAtlas.h"

class World {
public:
    World();
    explicit World(BlockAtlas* atlas);

    void genChunk(glm::vec3* pos);
    void newChunk(glm::vec3* pos, BlockChunk* c);

    void update();

    std::map<glm::vec3*, MeshChunk*>* getMeshChunks();

    ~World() = default;

    struct MeshThreadData {
        MeshThreadData(glm::vec3* pos, BlockChunk* chunk, BlockAtlas* atlas);

        std::thread* thread;

        glm::vec3* pos;
        BlockChunk* chunk;
        BlockAtlas* atlas;

        bool done;

        std::vector<float>* vertices;
        std::vector<unsigned int>* indices;

        ~MeshThreadData();
    };

    struct ChunkThreadData {
        ChunkThreadData(glm::vec3* pos, BlockAtlas* atlas);

        std::thread* thread;

        glm::vec3* pos;
        BlockAtlas* atlas;

        bool done;

        BlockChunk* chunk;

        ~ChunkThreadData();
    };
private:
    //Note to self:
    //The same glm::vec3 pointer is used for the blockChunk and meshChunk maps. If discarding a block/meshChunk, keep
    //this in mind. If this needs to be changed later change the 'update' method.
    std::map<glm::vec3*, BlockChunk*> blockChunks;
    std::map<glm::vec3*, MeshChunk*> meshChunks;

    //TODO: Replace this BiQueueThreadArray model with a BiQueueThreadPool model (it's in the name)

    //Chunk Gen BiQueue Variables
    const int MAX_CHUNK_GEN_THREADS = 16;
    std::unordered_set<glm::vec3*> chunkGenQueue;
    std::vector<ChunkThreadData*> chunkGenThreads;

    //Mesh Gen BiQueue Variables
    const int MAX_MESH_GEN_THREADS = 64;
    std::unordered_set<glm::vec3*> meshGenQueue;
    std::vector<MeshThreadData*> meshGenThreads;

    BlockAtlas* blockAtlas;
};


#endif //GLPROJECT_WORLD_H
