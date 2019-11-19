//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

#include "../../../world/LocalDimension.h"

MeshGenStream::MeshGenStream(LocalDefs &defs, LocalDimension &dimension) :
    defs(defs),
    dimension(dimension),
    noiseSampler({NoiseSample(1, 1), NoiseSample(1, 1), NoiseSample(1, 1)}) {

    queuedTasks.reserve(static_cast<unsigned long>(TOTAL_QUEUE_SIZE));

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
        threads.emplace_back(defs, noiseSampler);
    }
}

MeshGenStream::Thread::Thread(LocalDefs &defs, std::array<NoiseSample, 3>& offsetSamplers) :
    defs(defs), offsetSamplers(offsetSamplers) {
    thread = std::thread(MeshGenStream::threadFunction, this);
}

std::vector<MeshDetails*> MeshGenStream::update() {
    std::vector<MeshDetails*> finishedChunks;

    for (Thread& t : threads) {
        for (Unit& u : t.tasks) {
            if (u.busy) continue;

            if (u.thisChunk != nullptr) {
                u.thisChunk = nullptr;
                finishedChunks.push_back(u.meshDetails);
                u.meshDetails = new MeshDetails();
            }

            if (!queuedTasks.empty()) {
                glm::vec3 pos = *queuedTasks.begin();
                queuedTasks.erase(queuedTasks.begin());
                queuedMap.erase(pos);

                u.meshDetails->pos = pos;
                std::shared_ptr<BlockChunk> chunk = dimension.getChunk(pos);
                if (chunk == nullptr) goto breakAddTask;

                u.thisChunk = shared_ptr<BlockChunk>(chunk);

                int ind = 0;
                for (glm::vec3 dir : VecUtils::getCardinalVectors()) {
                    std::shared_ptr<BlockChunk> adjacent = dimension.getChunk(pos + dir);
                    u.adjacentChunks[ind++] = shared_ptr<BlockChunk>(adjacent);
                    if (adjacent == nullptr) goto breakAddTask;
                }

                u.busy = true;
            }

            breakAddTask:;
        }
    }

    return std::move(finishedChunks);
}

void MeshGenStream::threadFunction(MeshGenStream::Thread *thread) {
    while (thread->keepAlive) {
        bool hasNoTasks = true;

        for (Unit& u : thread->tasks) {
            if (!u.busy) continue;

            MeshGenerator m(u.meshDetails, thread->defs, u.thisChunk, u.adjacentChunks, thread->offsetSamplers);
            hasNoTasks = false;
            u.busy = false;
        }

         if (hasNoTasks) std::this_thread::sleep_for(1ms);
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

MeshGenStream::~MeshGenStream() {
    for (auto& t : threads) {
        t.keepAlive = false;
        t.thread.join();
    }
}