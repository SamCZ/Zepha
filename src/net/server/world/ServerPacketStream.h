//
// Created by aurailus on 2020-07-16.
//

#pragma once

#include <queue>
#include <thread>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "../../Packet.h"
#include "../../../util/Vec.h"
#include "../../../world/chunk/MapBlock.h"

class ServerWorld;

class ServerPacketStream {
public:
    static const int THREADS = 4;
    static const int THREAD_QUEUE_SIZE = 64;

    struct FinishedJob {
        FinishedJob(glm::ivec4 pos, std::unique_ptr<Packet> packet) : pos(pos), packet(std::move(packet)) {}

        glm::ivec4 pos {};
        std::unique_ptr<Packet> packet;
    };

    ServerPacketStream(ServerWorld& world);

    bool queue(glm::ivec4 pos);
    std::unique_ptr<std::vector<std::unique_ptr<FinishedJob>>> update();

    ~ServerPacketStream();
private:
    struct Job {
        bool locked = false;

        glm::ivec4 pos {};
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

    ServerWorld& world;

    std::vector<Thread> threads;
    std::queue<glm::ivec4> queuedTasks;
    std::unordered_set<glm::ivec4, Vec::ivec4> queuedMap;
};