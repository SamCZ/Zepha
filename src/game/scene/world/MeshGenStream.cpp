//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

MeshGenStream::MeshGenStream(LocalDefs &defs, Dimension &dimension) :
    defs(defs),
    dimension(dimension),
    noiseSampler({NoiseSample(1, 1), NoiseSample(1, 1), NoiseSample(1, 1)}) {

    queuedTasks.reserve((unsigned long) TOTAL_QUEUE_SIZE);

    noise::module::Perlin offsetBaseNoise;
    offsetBaseNoise.SetFrequency(8);
    offsetBaseNoise.SetOctaveCount(3);

    noise::module::Turbulence offsetTurbulence;
    offsetTurbulence.SetSourceModule(0, offsetBaseNoise);
    offsetTurbulence.SetFrequency(4.0);
    offsetTurbulence.SetPower(0.125);

    //8 is just a random value to offset results
    noiseSampler = {
            NoiseSample::getSample(&offsetTurbulence, {0, 0, 8}, 16, 16),
            NoiseSample::getSample(&offsetTurbulence, {0, 8, 0}, 16, 16),
            NoiseSample::getSample(&offsetTurbulence, {8, 0, 0}, 16, 16)
    };

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) {
        threads.emplace_back(defs.blocks(), noiseSampler);
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

std::vector<MeshGenStream::MeshDetails> MeshGenStream::update() {
    std::vector<MeshDetails> finishedChunks;

    for (auto& t : threads) {
        for (auto& u : t.tasks) {
            if (!u.unlocked) continue;

            if (u.chunk != nullptr) {
                finishedChunks.emplace_back(u.vertices, u.indices, u.chunk->pos);
                u.chunk = nullptr;
            }

            if (!queuedTasks.empty()) {
                auto it = queuedTasks.begin();
                glm::vec3 pos = *it;
                queuedTasks.erase(it);
                queuedMap.erase(pos);

                u.chunk = dimension.getChunk(pos);
                u.adjacent = getAdjacentsCull(pos);

                if (u.adjacent == nullptr) {
                    //Some of the adjacent chunks have been GC'd since the task was queued
                    u.chunk = nullptr;
                }
                else {
                    //Lock it in to allow the thread to edit it.
                    u.unlocked = false;
                }
            }
        }
    }

    return std::move(finishedChunks);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
MeshGenStream::Thread::Thread(LocalBlockAtlas &atlas, std::array<NoiseSample, 3>& offsetSamplers) :
    atlas(atlas), offsetSamplers(offsetSamplers),
    thread(MeshGenStream::threadFunction, this) {}
#pragma clang diagnostic pop

void MeshGenStream::threadFunction(MeshGenStream::Thread *thread) {
    while (thread->keepAlive) {

        bool empty = true;
        for (Unit& u : thread->tasks) {
            if (!u.unlocked) {

                empty = false;

                u.vertices = new std::vector<ChunkVertex>();
                u.indices = new std::vector<unsigned int>();

                MeshGenerator(*u.vertices, *u.indices, thread->atlas, *u.chunk, *u.adjacent, thread->offsetSamplers);

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
        t.thread.join();
    }
}

std::vector<bool>* MeshGenStream::getAdjacentsCull(glm::vec3 pos) {
    auto culls = new std::vector<bool>();
    culls->reserve((TransPos::CHUNK_SIZE*TransPos::CHUNK_SIZE) * 6); //256 * 6

    auto vectors = VecUtils::getCardinalVectors();
    for (int i = 0; i < vectors.size(); i++) {
        auto chunk = dimension.getChunk(pos + vectors[i]);

        if (chunk == nullptr) {
            return nullptr;
        }

        glm::vec3 checkPos;
        for (int j = 0; j < TransPos::CHUNK_SIZE; j++) {
            for (int k = 0; k < TransPos::CHUNK_SIZE; k++) {
                checkPos = {
                    (i == 0) ? 0 : (i == 1) ? (TransPos::CHUNK_SIZE-1) : (i <= 3) ? j : k,
                    (i == 2) ? 0 : (i == 3) ? (TransPos::CHUNK_SIZE-1) : j,
                    (i == 4) ? 0 : (i == 5) ? (TransPos::CHUNK_SIZE-1) : k
                };

                auto block = chunk->getBlock(checkPos);
                culls->push_back(defs.blocks().fromIndex(block).culls);
            }
        }
    }
    return culls;
}