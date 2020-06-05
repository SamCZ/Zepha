//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

#include "graph/ChunkMeshGenerator.h"
#include "../../../world/LocalDimension.h"

MeshGenStream::MeshGenStream(ClientGame& game, LocalDimension &dimension) :
    game(game),
    dimension(dimension) {

    queuedTasks.reserve(static_cast<unsigned long>(TOTAL_QUEUE_SIZE));

    noise::module::Perlin offsetBaseNoise;
    offsetBaseNoise.SetFrequency(8);
    offsetBaseNoise.SetOctaveCount(3);

    noise::module::Turbulence offsetTurbulence;
    offsetTurbulence.SetSourceModule(0, offsetBaseNoise);
    offsetTurbulence.SetFrequency(4.0);
    offsetTurbulence.SetPower(0.125);

    noiseSampler = { NoiseSample {}, NoiseSample {}, NoiseSample {} };
    //8 is just a random value to offset results
    noiseSampler[0].fill([&](glm::ivec3 pos) -> float { return offsetTurbulence.GetValue(pos.x + 8, pos.y, pos.z); }, {16, 16});
    noiseSampler[1].fill([&](glm::ivec3 pos) -> float { return offsetTurbulence.GetValue(pos.x, pos.y + 8, pos.z); }, {16, 16});
    noiseSampler[2].fill([&](glm::ivec3 pos) -> float { return offsetTurbulence.GetValue(pos.x, pos.y, pos.z + 8); }, {16, 16});

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(game, noiseSampler);
}

std::vector<ChunkMeshDetails*> MeshGenStream::update() {
    std::vector<ChunkMeshDetails*> finishedChunks;

    for (Thread& t : threads) {
        for (Unit& u : t.tasks) {
            if (u.busy) continue;

            if (u.thisChunk != nullptr) {
                u.thisChunk = nullptr;
                finishedChunks.push_back(u.meshDetails);
                u.meshDetails = new ChunkMeshDetails();
            }

            if (!queuedTasks.empty()) {
                glm::ivec3 pos = *queuedTasks.begin();
                queuedTasks.erase(queuedTasks.begin());
                queuedMap.erase(pos);

                u.meshDetails->pos = pos;
                std::shared_ptr<BlockChunk> chunk = dimension.getChunk(pos);
                if (chunk == nullptr) goto breakAddTask;

                u.thisChunk = std::shared_ptr<BlockChunk>(chunk);

                int ind = 0;
                for (const glm::ivec3& dir : Vec::adj) {
                    std::shared_ptr<BlockChunk> adjacent = dimension.getChunk(pos + dir);
                    u.adjacentChunks[ind++] = std::shared_ptr<BlockChunk>(adjacent);
                    if (adjacent == nullptr) goto breakAddTask;
                }

                u.busy = true;
            }

            breakAddTask:;
        }
    }

    return std::move(finishedChunks);
}

MeshGenStream::Thread::Thread(ClientGame &defs, std::array<NoiseSample, 3>& offsetSamplers) :
    game(defs), offsetSamplers(offsetSamplers), thread(std::bind(&MeshGenStream::Thread::exec, this)) {}

void MeshGenStream::Thread::exec() {
    while (keepAlive) {
        bool hasNoTasks = true;

        for (auto i = 0; i < tasks.size(); i++) {
            auto& u = tasks[i];
            if (!u.busy) continue;

            if (u.thisChunk == nullptr) {
                std::cout << Util::vecToString(u.thisChunk->pos) << std::endl;
                continue;
            }

            ChunkMeshGenerator m(u.meshDetails, game.defs, game.biomes, u.thisChunk, u.adjacentChunks, offsetSamplers);
            hasNoTasks = false;
            u.busy = false;
        }

         if (hasNoTasks) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool MeshGenStream::spaceInQueue() {
    return queuedTasks.size() < TOTAL_QUEUE_SIZE;
}

bool MeshGenStream::isQueued(glm::ivec3 pos) {
    return static_cast<bool>(queuedMap.count(pos));
}

bool MeshGenStream::tryToQueue(glm::ivec3 pos) {
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