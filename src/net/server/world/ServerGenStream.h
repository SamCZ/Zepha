//
// Created by aurailus on 06/03/19.
//

#pragma once

#include <queue>
#include <thread>
#include <unordered_set>

#include "../../../def/gen/MapGen.h"

class ServerGame;

class ServerGenStream {
public:
    static const int THREADS = 6;
    static const int THREAD_QUEUE_SIZE = 6;

    struct FinishedBlockJob {
        glm::ivec3 pos;
        std::vector<std::shared_ptr<Chunk>> chunks;
    };

    explicit ServerGenStream(unsigned int seed, ServerGame& game);
    ~ServerGenStream();

    // Add the MapBlock position `pos` to the generation queue.
    bool queue(glm::vec3 pos);
    // Returns a vector of BlockChunks that have finished generating,
    // and gives the threads new data to work with.
    std::unique_ptr<std::vector<FinishedBlockJob>> update();
private:
    struct Job {
        bool locked = false;

        glm::ivec3 pos {};
        std::unique_ptr<MapGen::ChunkMap> generated = nullptr;
    };

    struct Thread {
        explicit Thread(ServerGame& game, unsigned int seed);
        void exec();

        bool kill = false;

        std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);

        MapGen gen;
        std::thread thread;
    };

    std::vector<Thread> threads;
    std::queue<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};