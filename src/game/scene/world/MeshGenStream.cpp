//
// Created by aurailus on 27/03/19.
//

#include "MeshGenStream.h"

#include "ChunkMeshDetails.h"
#include "graph/ChunkMeshGenerator.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../world/LocalDimension.h"

MeshGenStream::MeshGenStream(ClientGame& game, LocalDimension &dimension) :
    game(game),
    dimension(dimension),
    noiseSampler({NoiseSample {16}, NoiseSample {16}, NoiseSample {16}}) {

    noise::module::Perlin offsetBaseNoise;
    offsetBaseNoise.SetFrequency(8);
    offsetBaseNoise.SetOctaveCount(3);

    noise::module::Turbulence offsetTurbulence;
    offsetTurbulence.SetSourceModule(0, offsetBaseNoise);
    offsetTurbulence.SetFrequency(4.0);
    offsetTurbulence.SetPower(0.125);

    // 8 is just a random value to offset results
    noiseSampler[0].populate([&](glm::ivec3 pos) { return offsetTurbulence.GetValue(pos.x + 8, pos.y, pos.z); });
    noiseSampler[1].populate([&](glm::ivec3 pos) { return offsetTurbulence.GetValue(pos.x, pos.y + 8, pos.z); });
    noiseSampler[2].populate([&](glm::ivec3 pos) { return offsetTurbulence.GetValue(pos.x, pos.y, pos.z + 8); });

    threads.reserve(THREADS);
    for (int i = 0; i < THREADS; i++) threads.emplace_back(game, noiseSampler);
}

std::vector<ChunkMeshDetails*> MeshGenStream::update() {
    std::vector<ChunkMeshDetails*> finishedChunks;

    for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
        for (Thread& t : threads) {
            auto& j = t.jobs[i];
            if (j.busy) continue;

            if (j.thisChunk) {
                j.thisChunk = nullptr;
                finishedChunks.push_back(j.meshDetails);
                j.meshDetails = new ChunkMeshDetails();
            }

            if (!queuedTasks.empty()) {
                auto pos = queuedTasks.front();
                queuedMap.erase(pos);
                queuedTasks.pop_front();

                std::shared_ptr<Chunk> chunk = dimension.getChunk(pos);
                if (chunk == nullptr) goto breakAddTask;

                j.meshDetails->pos = pos;
                j.thisChunk = std::shared_ptr<Chunk>(chunk);

                int ind = 0;
                for (const glm::ivec3& dir : Vec::TO_VEC) {
                    std::shared_ptr<Chunk> adjacent = dimension.getChunk(pos + dir);
                    j.adjacentChunks[ind++] = std::shared_ptr<Chunk>(adjacent);
                    if (adjacent == nullptr) goto breakAddTask;
                }

                j.busy = true;

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
        bool empty = true;

        for (auto i = 0; i < jobs.size(); i++) {
            auto& u = jobs[i];
            if (!u.busy) continue;

            ChunkMeshGenerator m(u.meshDetails, game.defs, game.biomes, u.thisChunk, u.adjacentChunks, offsetSamplers);
            empty = false;
            u.busy = false;
        }

         if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void MeshGenStream::queue(glm::ivec3 pos, bool priority) {
    if (!queuedMap.count(pos)) {
        priority ? queuedTasks.push_front(pos) : queuedTasks.push_back(pos);
        queuedMap.insert(pos);
    }
}

MeshGenStream::~MeshGenStream() {
    for (auto& t : threads) {
        t.keepAlive = false;
        t.thread.join();
    }
}