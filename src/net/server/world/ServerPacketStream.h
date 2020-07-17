//
// Created by aurailus on 2020-07-16.
//

#pragma once

#include <list>
#include <vector>
#include <thread>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "../../Packet.h"
#include "../../../util/Vec.h"
#include "../../../world/chunk/MapBlock.h"

class ServerDimension;

class ServerPacketStream {
public:
    static const int THREADS = 4;
    static const int THREAD_QUEUE_SIZE = 64;

    struct FinishedJob {
        FinishedJob(glm::ivec3 pos, std::unique_ptr<Packet> packet) : pos(pos), packet(std::move(packet)) {}

        glm::ivec3 pos {};
        std::unique_ptr<Packet> packet;
    };

    ServerPacketStream(ServerDimension& dimension);

    bool queue(glm::ivec3 pos);
    std::unique_ptr<std::vector<std::unique_ptr<FinishedJob>>> update();

    ~ServerPacketStream();
private:
    struct Job {
        bool locked = false;

        glm::ivec3 pos {};
        std::unique_ptr<Packet> packet = nullptr;
        std::shared_ptr<MapBlock> mapBlock = nullptr;
    };

    struct Thread {
        Thread();
        void exec();

        bool kill = false;

        std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);

        std::thread thread;
    };

    ServerDimension& dimension;

    std::vector<Thread> threads;
    std::list<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};