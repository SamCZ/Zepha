//
// Created by aurailus on 06/03/19.
//

#include "ServerGenStream.h"

#include "../../def/ServerGame.h"

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

std::unique_ptr<std::vector<std::shared_ptr<BlockChunk>>> ServerGenStream::update() {
    auto finishedChunks = std::make_unique<std::vector<std::shared_ptr<BlockChunk>>>();

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (u.locked) continue;

            if (!u.chunks.empty()) {
                for (auto chunk : u.chunks)
                    finishedChunks->push_back(std::shared_ptr<BlockChunk>(chunk.second.second));
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

ServerGenStream::Thread::Thread(MapGen *gen) : gen(gen) {
    thread = std::thread(ServerGenStream::threadFunction, this);
    thread.detach();
}

void ServerGenStream::threadFunction(ServerGenStream::Thread *thread) {
    while (!thread->kill) {

        bool empty = true;
        for (Job& u : thread->tasks) {
            if (u.locked) {
                empty = false;
                u.chunks = thread->gen->generateMapBlock(u.pos);
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