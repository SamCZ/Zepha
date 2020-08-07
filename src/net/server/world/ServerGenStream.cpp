//
// Created by aurailus on 06/03/19.
//

#include "ServerGenStream.h"

#include "ServerWorld.h"
#include "../../../def/ServerSubgame.h"
#include "../../../world/chunk/Chunk.h"

ServerGenStream::ServerGenStream(ServerSubgame& game, ServerWorld& world, unsigned int seed) {
    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(game, world, seed);
}

bool ServerGenStream::queue(unsigned int dimension, glm::ivec3 pos) {
    auto v4 = glm::ivec4(pos, dimension);
    if (!queuedMap.count(v4)) {
        queuedTasks.push(v4);
        queuedMap.insert(v4);
        return true;
    }
    return false;
}

std::unique_ptr<std::vector<ServerGenStream::FinishedJob>> ServerGenStream::update() {
    auto created = std::make_unique<std::vector<FinishedJob>>();

    for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
        for (auto& t : threads) {
            auto& j = t.jobs[i];
            if (j.locked) continue;

            if (j.created) created->push_back({j.pos, j.dim, std::move(j.created)});

            if (!queuedTasks.empty()) {
                auto pos = queuedTasks.front();
                queuedMap.erase(pos);
                queuedTasks.pop();

                j.pos = glm::ivec3(pos);
                j.dim = pos.w;
                j.locked = true;

            }
        }
    }

    return created;
}

ServerGenStream::Thread::Thread(ServerSubgame& game, ServerWorld& world, unsigned int seed) :
    gen(static_cast<Subgame&>(game), static_cast<World&>(world), seed), thread(std::bind(&ServerGenStream::Thread::run, this)) {}

void ServerGenStream::Thread::run() {
    while (!kill) {
        bool empty = true;
        for (Job& j : jobs) {
            if (j.locked) {
                empty = false;
                j.created = gen.generateMapBlock(j.dim, j.pos);
                j.locked = false;
            }
        }
        if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

ServerGenStream::~ServerGenStream() {
    for (auto& t : threads) {
        t.kill = true;
        t.thread.join();
    }
}