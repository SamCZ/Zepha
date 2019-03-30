//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

MeshGenStream::MeshGenStream() = default;

MeshGenStream::MeshGenStream(BlockAtlas* atlas) {
    queuedTasks.reserve((unsigned long) TOTAL_QUEUE_SIZE);

    this->atlas = atlas;

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(this->atlas);
    }
}

bool MeshGenStream::spaceInQueue() {
    return queuedTasks.size() < TOTAL_QUEUE_SIZE;
}

bool MeshGenStream::isQueued(glm::vec3 pos) {
    return (bool) queuedMap.count(pos);
}

bool MeshGenStream::tryToQueue(std::pair<BlockChunk*, std::vector<bool>*> data) {
    unsigned long sizeOfQueue = queuedTasks.size();

    if (sizeOfQueue < TOTAL_QUEUE_SIZE && !queuedMap.count(data.first->pos)) {
        queuedTasks.push_back(data);
        queuedMap.insert(data.first->pos);
    }

    return sizeOfQueue + 1 < TOTAL_QUEUE_SIZE;
}

std::vector<MeshGenStream::MeshDetails>* MeshGenStream::update() {
    auto finishedChunks = new std::vector<MeshDetails>;

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (!u.unlocked) continue;

            if (u.chunk != nullptr) {
                finishedChunks->emplace_back(u.vertices, u.indices, u.chunk->pos);
                u.chunk = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto it = queuedTasks.begin();
                auto data = *it;
                queuedTasks.erase(it);
                queuedMap.erase(data.first->pos);

                u.chunk = data.first;
                u.adjacent = data.second;

                //Lock it in to allow the thread to edit it.
                u.unlocked = false;
            }
        }
    }

    return finishedChunks;
}

MeshGenStream::Thread::Thread(BlockAtlas* atlas) {
    this->atlas = atlas;

    thread = new std::thread(MeshGenStream::threadFunction, this);
    thread->detach();
}

void MeshGenStream::threadFunction(MeshGenStream::Thread *thread) {
    while (thread->keepAlive) {

        bool empty = true;
        for (Unit& u : thread->tasks) {
            if (!u.unlocked) {

                empty = false;

                u.vertices = new std::vector<float>();
                u.indices = new std::vector<unsigned int>();

                MeshGenerator().build(u.chunk, thread->atlas, u.adjacent, *u.vertices, *u.indices);

                delete u.adjacent;

                u.unlocked = true;
                break;

            }
        }

        if (empty) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

MeshGenStream::~MeshGenStream() {
    for (auto& t : threads) {
        t.keepAlive = false;
        t.thread->join();
        delete t.thread;
    }
}

#pragma clang diagnostic pop