//
// Created by aurailus on 16/03/19.
//

#include "WorldInterpolationStream.h"

#include "../../../net/PacketView.h"
#include "../../../def/ClientGame.h"
#include "../../../world/chunk/Chunk.h"

WorldInterpolationStream::WorldInterpolationStream(unsigned int seed, ClientGame& game) {
    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(game, seed);
}

void WorldInterpolationStream::queuePacket(std::unique_ptr<PacketView> p) {
    queuedPacketTasks.push_back(std::move(p));
}

bool WorldInterpolationStream::queuePosition(glm::vec3 pos){
    if (!queuedInterpMap.count(pos)) {
        queuedInterpTasks.push_back(pos);
        queuedInterpMap.insert(pos);
        return true;
    }
    return false;
}

std::unique_ptr<std::vector<std::shared_ptr<Chunk>>> WorldInterpolationStream::update() {
    auto finishedChunks = std::make_unique<std::vector<std::shared_ptr<Chunk>>>();
//    auto finishedMapBlocks = std::make_unique<std::vector<std::shared_ptr<MeshFarMap>>>();

    for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
        for (auto& t : threads) {
            auto& j = t.jobs[i];
            if (j.locked) continue;

            if (j.chunk != nullptr) {
                finishedChunks->push_back(j.chunk);
                j.chunk = nullptr;
                j.job = JobType::EMPTY;
            }
//            else if (u.mapblock != nullptr) {
//                finishedMapBlocks->push_back(u.mapblock);
//                u.mapblock = nullptr;
//                u.job = JobType::EMPTY;
//            }

            if (!queuedPacketTasks.empty()) {
                auto it = queuedPacketTasks.begin();
                auto packet = std::move(*it);
                queuedPacketTasks.erase(it);

                j.job = JobType::PACKET;
                j.packet = std::move(packet);
                j.locked = true;
            }
            else if (!queuedInterpTasks.empty()) {
                auto it = queuedInterpTasks.begin();
                glm::vec3 pos = *it;
                queuedInterpTasks.erase(it);
                queuedInterpMap.erase(pos);

                j.job = JobType::FARMAP;
                j.mapBlockPos = pos;
                j.locked = true;
            }
        }
    }

    return finishedChunks;
}

WorldInterpolationStream::Thread::Thread(ClientGame& game, unsigned int seed) :
    gen(game.defs, game.biomes, seed),
    thread(std::bind(&WorldInterpolationStream::Thread::exec, this)) {}

void WorldInterpolationStream::Thread::exec() {
    while (!kill) {
        bool empty = true;
        for (Job& u : jobs) {
            if (u.locked) {
                if (u.job == JobType::PACKET) {
                    empty = false;
                    u.chunk = std::make_shared<Chunk>();
                    u.chunk->deserialize(u.packet->d);
                    u.locked = false;
                }
                else if (u.job == JobType::FARMAP) {
                    throw std::runtime_error("Farmap no exist yet.");
                }
//                    break;
            }
        }
        if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

WorldInterpolationStream::~WorldInterpolationStream() {
    for (auto& t : threads) {
        t.kill = true;
        t.thread.join();
    }
}