//
// Created by aurailus on 16/03/19.
//

#include "WorldInterpolationStream.h"

WorldInterpolationStream::WorldInterpolationStream(unsigned int seed) : gen(seed) {
    queuedTasks.reserve(1024);

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(&gen);
    }
}

bool WorldInterpolationStream::pushBack(Packet *p) {
    queuedTasks.push_back(p);
}

std::vector<std::shared_ptr<BlockChunk>> WorldInterpolationStream::update() {
    std::vector<std::shared_ptr<BlockChunk>> finishedChunks;

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (!u.unlocked) continue;

            if (u.chunk != nullptr) {
                finishedChunks.push_back(std::shared_ptr<BlockChunk>(u.chunk));
                u.chunk = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto it = queuedTasks.begin();
                Packet* p = *it;
                queuedTasks.erase(it);

                u.packet = p;
                //Lock it to allow the thread to edit it.
                u.unlocked = false;
            }
        }
    }

    return finishedChunks;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
WorldInterpolationStream::Thread::Thread(MapGen *gen) : gen(gen) {
    thread = std::thread(WorldInterpolationStream::threadFunction, this);
}
#pragma clang diagnostic pop

void WorldInterpolationStream::threadFunction(WorldInterpolationStream::Thread *thread) {

    while (thread->keepAlive) {

        bool noJobs = true;

        for (Unit& u : thread->tasks) {
            if (!u.unlocked) {
                noJobs = false;

                auto pos = Serializer::decodeIntVec3(&u.packet->data[0]);
                int len = Serializer::decodeInt(&u.packet->data[12]);

                std::string data(u.packet->data.begin() + 16, u.packet->data.begin() + 16 + len);

                u.chunk = new BlockChunk;
                u.chunk->pos = pos;
                u.chunk->deserialize(data);

                delete u.packet;
                u.packet = nullptr;

                u.unlocked = true;
                break;
            }
        }

        if (noJobs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

WorldInterpolationStream::~WorldInterpolationStream() {
    for (auto& t : threads) {
        t.keepAlive = false;
        t.thread.join();
    }
}