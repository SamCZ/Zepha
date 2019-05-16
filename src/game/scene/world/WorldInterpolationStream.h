//
// Created by aurailus on 16/03/19.
//

#ifndef ZEUS_WORLDINTERPOLATIONSTREAM_H
#define ZEUS_WORLDINTERPOLATIONSTREAM_H


#include <vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/gen/MapGen.h"

class WorldInterpolationStream {
public:
    static const int THREAD_QUEUE_SIZE = 32;
    static const int THREADS = 4;

    WorldInterpolationStream() : gen(0) {};
    explicit WorldInterpolationStream(unsigned int seed);
    ~WorldInterpolationStream();

    //Add `p` to the pre-thread queue.
    bool pushBack(Packet p);

    //Will return a vector of BlockChunk pointers containing finished chunks.
    //Frees up the threads and starts new tasks.
    std::vector<std::shared_ptr<BlockChunk>> update();

    struct Unit {
        Packet packet;
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
    std::vector<Packet> queuedTasks;
};


#endif //ZEUS_WORLDINTERPOLATIONSTREAM_H
