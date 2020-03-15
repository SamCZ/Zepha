//
// Created by aurailus on 16/03/19.
//

#pragma once

#include <thread>

#include "../../../def/ClientGame.h"
#include "../../../world/chunk/BlockChunk.h"

class WorldInterpolationStream {
public:
    static const int THREAD_QUEUE_SIZE = 128;
    static const int THREADS = 2;

    WorldInterpolationStream(unsigned int seed, ClientGame& defs);
    ~WorldInterpolationStream();

    //Add `p` to the pre-thread queue.
    bool pushBack(std::unique_ptr<PacketView> p);

    //Will return a vector of BlockChunk pointers containing finished chunks.
    //Frees up the threads and starts new tasks.
    std::vector<std::shared_ptr<BlockChunk>> update();

    struct Unit {
        std::unique_ptr<PacketView> packet;
        BlockChunk* chunk = nullptr;

        bool unlocked = true;
    };

    struct Thread {
//        explicit Thread(MapGen* gen);
        explicit Thread();

//        MapGen* gen;

        std::thread thread;
        bool keepAlive = true;

        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);
    };

    std::vector<Thread> threads;
private:
    static void threadFunction(Thread* thread);

//    MapGen gen;
    std::vector<std::unique_ptr<PacketView>> queuedTasks;
};

