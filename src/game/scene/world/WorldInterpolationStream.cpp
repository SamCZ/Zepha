//
// Created by aurailus on 16/03/19.
//

#include "WorldInterpolationStream.h"

#include "../../../net/PacketView.h"
#include "../../../def/ClientGame.h"
#include "../../../def/gen/MapGen.h"
#include "../../../world/chunk/Chunk.h"

WorldInterpolationStream::WorldInterpolationStream(unsigned int seed, ClientGame& game) :
    props(std::make_shared<MapGenProps>(seed)),
    gen(new MapGen(seed, game.defs, game.biomes, props)) {

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(gen);
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

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (u.locked) continue;

            if (u.chunk != nullptr) {
                finishedChunks->push_back(u.chunk);
                u.chunk = nullptr;
                u.job = JobType::EMPTY;
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

                u.job = JobType::PACKET;
                u.packet = std::move(packet);
                u.locked = true;
            }
            else if (!queuedInterpTasks.empty()) {
                auto it = queuedInterpTasks.begin();
                glm::vec3 pos = *it;
                queuedInterpTasks.erase(it);
                queuedInterpMap.erase(pos);

                u.job = JobType::FARMAP;
                u.mapBlockPos = pos;
                u.locked = true;
            }
        }
    }

    return finishedChunks;
}

WorldInterpolationStream::Thread::Thread(MapGen* gen) : gen(gen),
    thread(std::bind(&WorldInterpolationStream::Thread::exec, this)) {}

void WorldInterpolationStream::Thread::exec() {
    while (!kill) {
        bool empty = true;
        for (Job& u : tasks) {
            if (u.locked) {
                if (u.job == JobType::PACKET) {
                    empty = false;
                    u.chunk = std::make_shared<Chunk>();
                    u.chunk->deserialize(u.packet->d);
                    u.locked = false;
                    break;
                }
                else if (u.job == JobType::FARMAP) {
                    throw std::runtime_error("Farmap no exist yet.");
                }
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