//
// Created by aurailus on 06/03/19.
//

#include "ServerGenStream.h"

#include "../../../def/ServerGame.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../def/gen/MapGenProps.h"

ServerGenStream::ServerGenStream(unsigned int seed, ServerGame& game) :
    props(std::make_shared<MapGenProps>(seed)),
    gen(seed, game.defs, game.biomes, props) {

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(&gen);
}

bool ServerGenStream::queue(glm::vec3 pos) {
    if (!queuedMap.count(pos)) {
        queuedTasks.push_back(pos);
        queuedMap.insert(pos);
        return true;
    }
    return false;
}

std::unique_ptr<std::vector<ServerGenStream::FinishedBlockJob>> ServerGenStream::update() {
    auto finishedChunks = std::make_unique<std::vector<FinishedBlockJob>>();

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (u.locked) continue;

            if (!u.chunks.empty()) {
                finishedChunks->push_back({u.pos, {}});
                for (auto chunk : u.chunks) finishedChunks->back().chunks.push_back(std::shared_ptr<Chunk>(chunk.second.second));
                u.chunks.clear();
            }

            if (!queuedTasks.empty()) {
                auto it = queuedTasks.begin();
                glm::vec3 pos = *it;
                queuedTasks.erase(it);
                queuedMap.erase(pos);

                u.pos = pos;
                u.locked = true;
            }
        }
    }

    return finishedChunks;
}

ServerGenStream::Thread::Thread(MapGen *gen) : gen(gen),
    thread(std::bind(&ServerGenStream::Thread::exec, this)) {}

void ServerGenStream::Thread::exec() {
    while (!kill) {
        bool empty = true;
        for (Job& u : tasks) {
            if (u.locked) {
                empty = false;
                u.chunks = gen->generateMapBlock(u.pos);
                u.locked = false;
                break;
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