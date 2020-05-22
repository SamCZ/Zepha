//
// Created by aurailus on 06/03/19.
//

#pragma once

#include <list>
#include <thread>
#include <unordered_set>
#include <glm/vec3.hpp>

#include "../../def/gen/MapGen.h"

class ServerGame;

class ServerGenStream {
public:
    static const int THREADS = 4;
    static const int THREAD_QUEUE_SIZE = 6;

    struct FinishedBlockJob {
        glm::ivec3 pos;
        std::vector<std::shared_ptr<BlockChunk>> chunks;
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
        MapGen::chunk_partials_map chunks {};
    };

    struct Thread {
        explicit Thread(MapGen* gen);
        void exec();

        MapGen* gen;
        std::vector<Job> tasks = std::vector<Job>(THREAD_QUEUE_SIZE);

        bool kill = false;
        std::thread thread;
    };

    std::shared_ptr<MapGenProps> props;
    MapGen gen;

    std::vector<Thread> threads;
    std::list<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};