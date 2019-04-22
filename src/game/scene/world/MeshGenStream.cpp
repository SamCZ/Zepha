//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

MeshGenStream::MeshGenStream(GameDefs &defs, Dimension &dimension) :
    defs(defs),
    dimension(dimension) {

    queuedTasks.reserve((unsigned long) TOTAL_QUEUE_SIZE);

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(defs.blocks());
    }
}

bool MeshGenStream::spaceInQueue() {
    return queuedTasks.size() < TOTAL_QUEUE_SIZE;
}

bool MeshGenStream::isQueued(glm::vec3 pos) {
    return (bool) queuedMap.count(pos);
}

bool MeshGenStream::tryToQueue(glm::vec3 pos) {
    unsigned long sizeOfQueue = queuedTasks.size();

    if (sizeOfQueue < TOTAL_QUEUE_SIZE && !queuedMap.count(pos)) {
        queuedTasks.push_back(pos);
        queuedMap.insert(pos);
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
                glm::vec3 pos = *it;
                queuedTasks.erase(it);
                queuedMap.erase(pos);

                u.chunk = dimension.getChunk(pos);
                u.adjacent = getAdjacentsCull(pos);

                //Lock it in to allow the thread to edit it.
                u.unlocked = false;
            }
        }
    }

    return finishedChunks;
}

MeshGenStream::Thread::Thread(BlockAtlas &atlas) :
    atlas(atlas) {

    thread = new std::thread(MeshGenStream::threadFunction, this);
}

void MeshGenStream::threadFunction(MeshGenStream::Thread *thread) {
    while (thread->keepAlive) {

        bool empty = true;
        for (Unit& u : thread->tasks) {
            if (!u.unlocked) {

                empty = false;

                u.vertices = new std::vector<float>();
                u.indices = new std::vector<unsigned int>();

                MeshGenerator().build(u.chunk, thread->atlas, *u.adjacent, *u.vertices, *u.indices);

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
    }
}

std::vector<bool>* MeshGenStream::getAdjacentsCull(glm::vec3 pos) {
    auto culls = new std::vector<bool>();
    culls->reserve((TransPos::CHUNK_SIZE*TransPos::CHUNK_SIZE) * 6); //256 * 6

    auto vectors = VecUtils::getCardinalVectors();
    for (int i = 0; i < vectors.size(); i++) {
        auto chunk = dimension.getChunk(pos + vectors[i]);

        for (int j = 0; j < TransPos::CHUNK_SIZE; j++) {
            for (int k = 0; k < TransPos::CHUNK_SIZE; k++) {

                int x = (i == 0) ? 0 : (i == 1) ? (TransPos::CHUNK_SIZE-1) : (i <= 3) ? j : k;
                int y = (i == 2) ? 0 : (i == 3) ? (TransPos::CHUNK_SIZE-1) : j;
                int z = (i == 4) ? 0 : (i == 5) ? (TransPos::CHUNK_SIZE-1) : k;

                auto block = chunk->getBlock(x, y, z);
                culls->push_back(defs.blocks().getBlock(block).isCulling());
            }
        }
    }
    return culls;
}

#pragma clang diagnostic pop