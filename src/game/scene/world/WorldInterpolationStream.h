//
// Created by aurailus on 16/03/19.
//

#pragma once

#include <list>
#include <thread>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "graph/MeshFarMap.h"
#include "../../../def/gen/MapGen.h"
#include "../../../util/net/PacketView.h"

class Chunk;
class MapGen;
class ClientGame;
class MapGenProps;

class WorldInterpolationStream {
public:
    static const int THREADS = 4;
    static const int THREAD_QUEUE_SIZE = 32;

    WorldInterpolationStream(unsigned int seed, ClientGame& game);
    ~WorldInterpolationStream();

    // Queue parsing of packet `p`.
    void queuePacket(std::unique_ptr<PacketView> p);
    // Queue interpolation of Mapblock at `pos`.
    bool queuePosition(glm::vec3 pos);
    // Returns a vector of BlockChunks that have finished processing,
    // and gives the threads new data to work with.
    std::unique_ptr<std::vector<std::shared_ptr<Chunk>>> update();

private:
    enum class JobType {
        EMPTY,
        PACKET,
        FARMAP
    };

    struct Job {
        bool locked = false;
        JobType job = JobType::EMPTY;

        std::unique_ptr<PacketView> packet = nullptr;
        std::shared_ptr<Chunk> chunk = nullptr;

        std::shared_ptr<MeshFarMap> mapblock = nullptr;
        glm::vec3 mapBlockPos = {0, 0, 0};

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
    MapGen* gen;

    std::vector<Thread> threads;
    std::list<std::unique_ptr<PacketView>> queuedPacketTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedInterpMap;
    std::list<glm::vec3> queuedInterpTasks;
};

