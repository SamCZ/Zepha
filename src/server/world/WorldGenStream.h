//
// Created by aurailus on 06/03/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#pragma once

#include <glm/vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../util/Vec.h"
#include "../../def/gen/MapGen.h"
#include "../../world/chunk/BlockChunk.h"
#include "../../def/ServerDefs.h"

class WorldGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 4;
    static const int THREADS = 4;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    explicit WorldGenStream(unsigned int seed, ServerDefs& defs);
    ~WorldGenStream();

    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(glm::vec3 pos);

    //Will return a vector of BlockChunk pointers containing finished chunks.
    //Frees up the threads and starts new units.
    std::unique_ptr<std::vector<std::shared_ptr<BlockChunk>>> update();

    struct Unit {
        glm::ivec3 pos {};
        MapGen::chunk_partials_map chunks {};

        bool unlocked = true;
    };

    struct Thread {
        explicit Thread(MapGen* gen);

        MapGen* gen;

        std::thread thread;
        bool keepAlive = true;

        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);
    };

    std::vector<Thread> threads;
private:
    static void threadFunction(Thread* thread);

    MapGen gen;
    std::vector<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};

#pragma clang diagnostic pop