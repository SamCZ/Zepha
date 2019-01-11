//
// Created by aurailus on 14/12/18.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "World.h"
#include "../../engine/PerlinNoise.h"
#include "../../engine/helpers/ArrayTrans3D.h"

World::World(BlockAtlas *atlas) {
    blockAtlas = atlas;

    for (int i = 0; i < GEN_THREADS; i++) {
        genThreads.push_back(new ChunkThreadDef());
    }

    for (int i = 0; i < MESH_THREADS; i++) {
        meshThreads.push_back(new MeshThreadDef());
    }
}

void World::genNewChunk(glm::vec3 pos) {
    if (!blockChunks.count(pos)) {
        pendingGen.insert(pos);
    }
}

void World::commitChunk(glm::vec3 pos, BlockChunk *c) {
    blockChunks.insert(std::pair<glm::vec3, BlockChunk*>(pos, c));
    if (!c->isEmpty()) {
        pendingMesh.insert(pos);
    }
}

void World::remeshChunk(glm::vec3 pos) {
    pendingMesh.insert(pos);
}

void World::update() {
    Timer world("World update");

    //Create / Finalize BlockChunks
    handleChunkGenQueue();

    //Create / Finalize MeshChunks
    handleMeshGenQueue();

//    game.printElapsedMs();
}

int World::getBlock(glm::vec3 pos) {
    auto chunkPos = World::chunkVec(World::roundVec(pos));
    auto local = World::localVec(World::roundVec(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) {
        return chunk->getBlock(&local);
    }
    return -1;
}

void World::setBlock(glm::vec3 pos, int block) {
    auto chunkPos = World::chunkVec(World::roundVec(pos));
    auto local = World::localVec(World::roundVec(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) {
        if (chunk->setBlock(&local, block)) {
            remeshChunk(chunkPos);
        }
    }
}

bool World::solidAt(glm::vec3 pos) {
    return getBlock(pos) != 0;
}

BlockChunk* World::getChunk(glm::vec3 chunkPos) {
    if (blockChunks.count(chunkPos) == 1) {
        return blockChunks[chunkPos];
    }
    return nullptr;
}

void World::handleChunkGenQueue() {
    for (auto threadDef : genThreads) {
        std::unique_lock<std::mutex> lock(threadDef->lock, std::defer_lock);
        lock.lock();
        for (auto iter = threadDef->tasks.begin(); iter != threadDef->tasks.end();) {
            if (finishedGen.size() > GEN_FINISHED_SIZE) break;
            auto threadData = *iter;

            if (threadData->done) {
                finishedGen.push_back(threadData);
                iter = threadDef->tasks.erase(iter);
            } else iter++;
        }

        while (!pendingGen.empty() && threadDef->tasks.size() < GEN_QUEUE_SIZE) {
            auto it = pendingGen.begin();
            pendingGen.erase(it);
            glm::vec3 pos = *it;

            threadDef->tasks.push_back(new ChunkThreadData(*it, blockAtlas));
        }
    }

    Timer t("Chunk Initialization");
    int genUpdates = 0;
    for (auto iter = finishedGen.begin(); iter != finishedGen.end(); ) {
        if (t.elapsedNs() > 4000000) break;

        ChunkThreadData* threadData = *iter;

        commitChunk(threadData->pos, threadData->chunk);
        iter = finishedGen.erase(iter);
        delete threadData;

        genUpdates++;
    }
    lastGenUpdates = genUpdates;
//    t.printElapsedMs();
}

//Function that runs on each ChunkGenThread in the chunk generation threadpool.
//Takes a threadDef object which contains a vector of tasks to do, and infinitely loops, completing tasks and
//re-inserting them into the vector to be further manipulated by the main thread.
void World::chunkGenThread(World::ChunkThreadDef* threadDef) {
    PerlinNoise p(9);
    PerlinNoise p2(9);

    //Infinite loop
    while (true) {
        std::unique_lock<std::mutex> lock(threadDef->lock, std::defer_lock);
        lock.lock();

        //Find the first unfinished task
        World::ChunkThreadData* data = nullptr;
        for (auto iter = threadDef->tasks.begin(); iter != threadDef->tasks.end(); ) {
            if (!(*iter)->done) {
                data = (*iter);
                threadDef->tasks.erase(iter);
                break;
            }
            iter++;
        }

        lock.unlock();

        if (data != nullptr) {
            auto *blocks = new std::vector<int>();
            blocks->reserve(4096);

            glm::vec3 innerPos, pos;

            for (int ind = 0; ind < 4096; ind++) {
                ArrayTrans3D::indAssignVec(ind, &innerPos);

                pos.x = innerPos.x + data->pos.x * CHUNK_SIZE;
                pos.y = innerPos.y + data->pos.y * CHUNK_SIZE;
                pos.z = innerPos.z + data->pos.z * CHUNK_SIZE;

                double val = p.noise(pos.x / (double) 32, pos.z / (double) 32, 0) * 16;
                val *= p2.noise((pos.x + 16) / (double) 48, (pos.z + 16) / (double) 48, 0) * 8;
                val /= 16;
                val *= pow(p.noise(pos.x / (double) 64, pos.z / (double) 64, 0), 2) * 40 + 1;
                val -= pos.y;

                int block = (val > 0) ? (val > 1) ? (val > 3) ? 3 : 2 : 1 : 0;
                blocks->push_back(block);
            }
//
//            (*blocks)[ArrayTrans3D::vecToInd(8, 8, 8)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(7, 8, 8)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(7, 8, 7)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(8, 8, 7)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(8, 7, 8)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(7, 7, 8)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(7, 7, 7)] = 4;
//            (*blocks)[ArrayTrans3D::vecToInd(8, 7, 7)] = 4;

            data->chunk = new BlockChunk(blocks);
            data->done = true;

            lock.lock();
            threadDef->tasks.push_back(data);
            lock.unlock();
        }

        this_thread::sleep_for(1ms);
    }
}

void World::handleMeshGenQueue() {
    for (auto threadDef : meshThreads) {
        std::unique_lock<std::mutex> lock(threadDef->lock, std::defer_lock);
        lock.lock();
        for (auto iter = threadDef->tasks.begin(); iter != threadDef->tasks.end();) {
            if (finishedMesh.size() > MESH_FINISHED_SIZE) break;

            auto threadData = *iter;

            if (threadData->done) {
                finishedMesh.push_back(threadData);
                iter = threadDef->tasks.erase(iter);
            } else iter++;
        }

        while (!pendingMesh.empty() && threadDef->tasks.size() < MESH_QUEUE_SIZE) {
            auto it = pendingMesh.begin();
            pendingMesh.erase(it);
            glm::vec3 pos = *it;

            threadDef->tasks.push_back(new MeshThreadData(*it, blockChunks.at(pos), blockAtlas));
        }
    }

    Timer t("Mesh Initialization");
    int meshUpdates = 0;
    for (auto iter = finishedMesh.begin(); iter != finishedMesh.end(); ) {
        if (t.elapsedNs() > 4000000) break;

        MeshThreadData* threadData = *iter;

        if (meshChunks.count(threadData->pos) != 0) {
            meshChunks.erase(threadData->pos);
        }

        if (!threadData->vertices->empty()) {
            auto meshChunk = new MeshChunk();
            meshChunk->build(threadData->vertices, threadData->indices);

            glm::vec3 pos = threadData->pos * glm::vec3(CHUNK_SIZE);
            meshChunk->setPosition(pos);
            meshChunks.insert(std::pair<glm::vec3, MeshChunk *>(threadData->pos, meshChunk));
        }

        iter = finishedMesh.erase(iter);
        delete threadData;

        meshUpdates++;
    }
    lastMeshUpdates = meshUpdates;
//    t.printElapsedMs();
}

//Function that runs on each MeshGenThread in the mesh generation threadpool.
//Processes tasks and returns meshes in the same vector to be handled by the main thread.
void World::meshGenThread(MeshThreadDef* threadDef) {

    //Infinite loop
    while (true) {
        std::unique_lock<std::mutex> lock(threadDef->lock, std::defer_lock);
        lock.lock();

        //Find the first unfinished task
        World::MeshThreadData* data = nullptr;
        for (auto iter = threadDef->tasks.begin(); iter != threadDef->tasks.end(); ) {
            if (!(*iter)->done) {
                data = (*iter);
                threadDef->tasks.erase(iter);
                break;
            }
            iter++;
        }

        lock.unlock();

        if (data != nullptr) {
            MeshGenerator().build(data->chunk, data->atlas, *(data->vertices), *(data->indices));
            data->done = true;

            lock.lock();
            threadDef->tasks.push_back(data);
            lock.unlock();
        }

        this_thread::sleep_for(1ms);
    }
}

std::unordered_map<glm::vec3, MeshChunk*, World::vec3cmp>* World::getMeshChunks() {
    return &meshChunks;
}

//Constructors and Destructors for the MeshThreadData / ChunkThreadData structs.

World::ChunkThreadData::ChunkThreadData(glm::vec3 pos, BlockAtlas *atlas) {
    this->pos = pos;
    this->atlas = atlas;
    this->done = false;
    this->chunk = nullptr;
}

World::ChunkThreadDef::ChunkThreadDef() {
    thread = new std::thread(chunkGenThread, this);

    sched_param sch_params{};
    sch_params.sched_priority = 1;
    pthread_setschedparam(thread->native_handle(), SCHED_RR, &sch_params);

    thread->detach();
}

World::ChunkThreadDef::~ChunkThreadDef() {
    delete thread;
}

World::MeshThreadData::MeshThreadData(glm::vec3 pos, BlockChunk *chunk, BlockAtlas *atlas) {
    this->pos = pos;
    this->chunk = chunk;
    this->atlas = atlas;
    this->done = false;
    this->vertices = new std::vector<float>();
    this->indices = new std::vector<unsigned int>();
}

World::MeshThreadData::~MeshThreadData() {
    delete vertices;
    delete indices;
}

World::MeshThreadDef::MeshThreadDef() {
    thread = new std::thread(meshGenThread, this);

    sched_param sch_params{};
    sch_params.sched_priority = 1;
    pthread_setschedparam(thread->native_handle(), SCHED_RR, &sch_params);

    thread->detach();
}

World::MeshThreadDef::~MeshThreadDef() {
    delete thread;
}