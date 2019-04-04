//
// Created by aurailus on 06/03/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#ifndef ZEUS_WORLDGENSTREAM_H
#define ZEUS_WORLDGENSTREAM_H


#include <vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../generic/blocks/BlockChunk.h"
#include "../../generic/gen/MapGen.h"
#include "../../generic/helpers/VecUtils.h"

class WorldGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 32;
    static const int THREADS = 4;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    WorldGenStream() : gen(0) {};
    explicit WorldGenStream(unsigned int seed);
    ~WorldGenStream();

    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(glm::vec3 pos);

    //Will return a vector of BlockChunk pointers containing finished chunks.
    //Frees up the threads and starts new tasks.
    std::vector<BlockChunk*> update();

    struct Unit {
        glm::vec3 pos {0, 0, 0};
        BlockChunk* chunk = nullptr;

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
    std::unordered_set<glm::vec3, VecUtils::compareFunc> queuedMap;
};


#endif //ZEUS_WORLDGENSTREAM_H
#pragma clang diagnostic pop