//
// Created by aurailus on 06/03/19.
//

#pragma once

#include <queue>
#include <thread>
#include <shared_mutex>
#include <unordered_set>

#include "../../../def/gen/MapGen.h"
#include "../../../util/CovariantPtr.h"

class ServerSubgame;

class ServerGenStream {
public:
    static const int THREADS = 6;
    static const int THREAD_QUEUE_SIZE = 6;

    struct FinishedJob {
        glm::ivec3 pos;
        unsigned int dim;
        std::unique_ptr<MapGen::CreatedSet> created;
    };

    explicit ServerGenStream(ServerSubgame& game, ServerWorld& world, unsigned int seed);
    ~ServerGenStream();

    bool queue(unsigned int dim, glm::ivec3 pos);
    std::unique_ptr<std::vector<FinishedJob>> update();
private:
    struct Job {
        bool locked = false;
        std::unique_ptr<MapGen::CreatedSet> created = nullptr;

        glm::ivec3 pos {};
        unsigned int dim;
    };

    struct Thread {
        explicit Thread(ServerSubgame& game, ServerWorld& world, unsigned int seed);
        void run();

        bool kill = false;

        std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);

        MapGen gen;
        std::thread thread;
    };

    std::vector<Thread> threads;
    std::queue<glm::ivec4> queuedTasks;
    std::unordered_set<glm::ivec4, Vec::ivec4> queuedMap;
};