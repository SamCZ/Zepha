//
// Created by aurailus on 06/03/19.
//

#include "ServerGenStream.h"

#include "../../../def/ServerGame.h"
#include "../../../world/chunk/Chunk.h"

ServerGenStream::ServerGenStream(unsigned int seed, ServerGame& game) {
    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(game, seed);
}

bool ServerGenStream::queue(glm::vec3 pos) {
    if (!queuedMap.count(pos)) {
        queuedTasks.push(pos);
        queuedMap.insert(pos);
        return true;
    }
    return false;
}

std::unique_ptr<std::vector<ServerGenStream::FinishedBlockJob>> ServerGenStream::update() {
    auto finishedChunks = std::make_unique<std::vector<FinishedBlockJob>>();

    for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
        for (auto& t : threads) {
            auto& j = t.jobs[i];
            if (j.locked) continue;

            if (j.generated) {
                finishedChunks->push_back({j.pos, {}});
                for (auto chunkPair : *j.generated) finishedChunks->back().chunks.push_back(chunkPair.second);
                j.generated = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto pos = queuedTasks.front();
                queuedMap.erase(pos);
                queuedTasks.pop();

                j.pos = pos;
                j.locked = true;

            }
        }
    }

    return finishedChunks;
}

ServerGenStream::Thread::Thread(ServerGame& game, unsigned int seed) :
    gen(game.defs, game.biomes, seed),
    thread(std::bind(&ServerGenStream::Thread::exec, this)) {}

void ServerGenStream::Thread::exec() {
    while (!kill) {
        bool empty = true;
        for (Job& j : jobs) {
            if (j.locked) {
                empty = false;
                j.generated = gen.generateMapBlock(j.pos);
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