//
// Created by aurailus on 27/03/19.
//

#pragma once


#include <thread>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "ChunkMeshDetails.h"
#include "../../../util/Vec.h"
#include "../../../def/ClientGame.h"
#include "../../../def/gen/NoiseSample.h"
#include "../../../world/chunk/BlockChunk.h"

class LocalDimension;

class MeshGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 64;
    static const int THREADS = 2;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    explicit MeshGenStream(ClientGame& game, LocalDimension& dimension);
    ~MeshGenStream();

    bool spaceInQueue();
    bool isQueued(glm::ivec3 pos);
    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(glm::ivec3 pos);

    //Will return a vector of MeshDetails pointers containing finished meshes.
    //Frees up the threads and starts new tasks.
    std::vector<ChunkMeshDetails*> update();

    struct Unit {
        std::shared_ptr<BlockChunk> thisChunk = nullptr;
        std::array<std::shared_ptr<BlockChunk>, 6> adjacentChunks {};

        ChunkMeshDetails* meshDetails = new ChunkMeshDetails();

        bool busy = false;
    };

    struct Thread {
        explicit Thread(ClientGame &defs, std::array<NoiseSample, 3>& offsetSampler);
        void exec();

        ClientGame &game;
        std::array<NoiseSample, 3>& offsetSamplers;
        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);

        bool keepAlive = true;
        std::thread thread {};
    };

    std::vector<Thread> threads;
private:

    LocalDimension& dimension;
    ClientGame& game;

    std::array<NoiseSample, 3> noiseSampler;
    std::vector<glm::ivec3> queuedTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};

