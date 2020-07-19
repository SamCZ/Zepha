//
// Created by aurailus on 2020-07-16.
//

#include <iostream>
#include "ServerPacketStream.h"

#include "../../Serializer.h"
#include "../../../world/ServerDimension.h"
#include "../../../world/chunk/Chunk.h"

ServerPacketStream::ServerPacketStream(ServerDimension& dimension) : dimension(dimension) {
    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back();
}

bool ServerPacketStream::queue(glm::ivec3 pos) {
    if (!queuedMap.count(pos)) {
        queuedTasks.push(pos);
        queuedMap.insert(pos);
        return true;
    }
    return false;
}

ServerPacketStream::Thread::Thread() :
        thread(std::bind(&ServerPacketStream::Thread::exec, this)) {}

std::unique_ptr<std::vector<std::unique_ptr<ServerPacketStream::FinishedJob>>> ServerPacketStream::update() {
    auto finished = std::make_unique<std::vector<std::unique_ptr<FinishedJob>>>();

    for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
        for (auto& t : threads) {
            auto& j = t.jobs[i];
            if (j.locked) continue;

            if (j.packet) {
                finished->emplace_back(std::make_unique<FinishedJob>(j.pos, std::move(j.packet)));
                j.packet = nullptr;
                j.mapBlock = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto pos = queuedTasks.front();
                queuedMap.erase(pos);
                queuedTasks.pop();

                auto mapBlock = dimension.getMapBlock(pos);
                if (!mapBlock) continue;

                j.pos = pos;
                j.mapBlock = mapBlock;
                j.locked = true;
            }
        }
    }

    return finished;
}

void ServerPacketStream::Thread::exec() {
    while (!kill) {
        bool empty = true;
        for (Job& j : jobs) {
            if (j.locked) {
                empty = false;

                Serializer s {};
                for (unsigned int i = 0; i < 64; i++) {
                    auto chunk = (*j.mapBlock)[i];
                    auto l = chunk->aquireLock();
                    s.append(chunk->serialize());
                }

                j.packet = std::make_unique<Packet>(PacketType::MAPBLOCK);
                j.packet->data = s.data;
                j.locked = false;
            }
        }
        if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


ServerPacketStream::~ServerPacketStream() {
    for (auto& t : threads) {
        t.kill = true;
        t.thread.join();
    }
}