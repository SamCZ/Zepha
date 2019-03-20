//
// Created by aurailus on 06/03/19.
//

#include "WorldGenStream.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

WorldGenStream::WorldGenStream(unsigned int seed) : gen(seed) {
    queuedTasks.reserve((unsigned long) TOTAL_QUEUE_SIZE);

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(&gen);
    }
}

bool WorldGenStream::tryToQueue(glm::vec3 pos) {
    unsigned long sizeOfQueue = queuedTasks.size();

    if (sizeOfQueue < TOTAL_QUEUE_SIZE && !queuedMap.count(pos)) {
        queuedTasks.push_back(pos);
        queuedMap.insert(pos);
    }

    return sizeOfQueue + 1 < TOTAL_QUEUE_SIZE;
}

std::vector<BlockChunk*> WorldGenStream::update() {
    std::vector<BlockChunk*> finishedChunks;

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (!u.unlocked) continue;

            if (u.chunk != nullptr) {
                finishedChunks.push_back(u.chunk);
                u.chunk = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto it = queuedTasks.begin();
                glm::vec3 pos = *it;
                queuedTasks.erase(it);
                queuedMap.erase(pos);

                u.pos = pos;
                //Lock it in to allow the thread to edit it.
                u.unlocked = false;
            }
        }
    }

    return finishedChunks;
}

WorldGenStream::Thread::Thread(MapGen *gen) {
    this->gen = gen;

    thread = std::thread(WorldGenStream::threadFunction, this);
    thread.detach();
}

void WorldGenStream::threadFunction(WorldGenStream::Thread *thread) {
    while (thread->keepAlive) {

        bool empty = true;
        for (Unit& u : thread->tasks) {
            if (!u.unlocked) {

                empty = false;
                u.chunk = thread->gen->generate(u.pos);
                u.unlocked = true;
                break;

            }
        }

        if (empty) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

WorldGenStream::~WorldGenStream() {
    for (auto& t : threads) {
        t.keepAlive = false;
        t.thread.join();
    }
}

#pragma clang diagnostic pop