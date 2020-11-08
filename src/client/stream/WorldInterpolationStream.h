//
// Created by aurailus on 16/03/19.
//

#pragma once

#include <queue>
#include <thread>
#include <memory>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "util/Vec.h"
#include "world/gen/MapGen.h"
#include "util/CovariantPtr.h"

class Chunk;
class LocalSubgame;
class PacketView;

class WorldInterpolationStream {
public:
    static const int THREADS = 4;
    static const int THREAD_QUEUE_SIZE = 16;

    WorldInterpolationStream(LocalSubgame& game, LocalWorld& world, unsigned int seed);

    // Queue parsing of packet `p`.
    void queuePacket(std::unique_ptr<PacketView> p);
    // Queue interpolation of Mapblock at `pos`.
//    bool queuePosition(glm::vec3 pos);
    // Returns a vector of BlockChunks that have finished processing,
    // and gives the threads new data to work with.
    std::unique_ptr<std::vector<std::shared_ptr<Chunk>>> update();

    ~WorldInterpolationStream();
private:
//    enum class JobType {
//        EMPTY,
//        PACKET,
//        FARMAP
//    };

    struct Job {
        bool locked = false;
//        JobType job = JobType::EMPTY;

        std::shared_ptr<PacketView> packet = nullptr;
        std::vector<std::shared_ptr<Chunk>> chunks = {};

//        std::shared_ptr<MeshFarMap> mapblock = nullptr;
//        glm::vec3 mapBlockPos = {0, 0, 0};
    };

    struct Thread {
        explicit Thread(LocalSubgame& game, LocalWorld& world, unsigned int seed);
        void run();

        bool kill = false;

        std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);

        std::thread thread;
    };

    std::vector<Thread> threads;
    std::queue<std::unique_ptr<PacketView>> queuedPacketTasks;
//    std::unordered_set<glm::vec3, Vec::vec3> queuedInterpMap;
//    std::queue<glm::vec3> queuedInterpTasks;
};

