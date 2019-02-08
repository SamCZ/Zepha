//
// Created by aurailus on 22/01/19.
//

#ifndef ZEUS_WORLDTHREADDEFS_H
#define ZEUS_WORLDTHREADDEFS_H

#include <vec3.hpp>
#include <thread>
#include <mutex>

#include "../../../generic/blocks/BlockChunk.h"
#include "../../../generic/blocks/BlockAtlas.h"
#include "../../../generic/gen/MapGen.h"

//Structs for storing the threads used in World, and passing data to and from them.

struct MeshThreadData {
    MeshThreadData(glm::vec3 pos, BlockChunk* chunk, std::vector<bool>* adjacents, BlockAtlas* atlas);

    glm::vec3 pos;
    BlockChunk* chunk;
    BlockAtlas* atlas;
    std::vector<bool>* adjacents;

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
    explicit ChunkThreadDef(MapGen *mapGen);

    std::thread* thread;
    std::mutex lock;
    std::vector<ChunkThreadData*> tasks;

    MapGen* mapGen;

    ~ChunkThreadDef();
};


#endif //ZEUS_WORLDTHREADDEFS_H
