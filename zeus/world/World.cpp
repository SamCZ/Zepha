//
// Created by aurailus on 14/12/18.
//

#include "World.h"
#include "../engine/PerlinNoise.h"
#include "../engine/helpers/ArrayTrans3D.h"

World::World() {
    blockAtlas = nullptr;
}

World::World(BlockAtlas *atlas) {
    blockAtlas = atlas;
}

void World::genChunk(glm::vec3 pos) {
    if (!blockChunks.count(pos)) {
        chunkGenQueue.insert(pos);
    }
}

void World::newChunk(glm::vec3 pos, BlockChunk *c) {
    blockChunks.insert(std::pair<glm::vec3, BlockChunk*>(pos, c));
    if (!c->isEmpty()) {
        meshGenQueue.insert(pos);
    }
}

void chunkGenThread(World::ChunkThreadData *t) {
    PerlinNoise p(0);
    PerlinNoise p2(10);

    auto* blocks = new std::vector<int>();
    blocks->reserve(4096);

     glm::vec3 innerPos, pos;

     for (int ind = 0; ind < 4096; ind++) {
          ArrayTrans3D::indAssignVec(ind, &innerPos);

          pos.x = innerPos.x + t->pos.x * CHUNK_SIZE;
          pos.y = innerPos.y + t->pos.y * CHUNK_SIZE;
          pos.z = innerPos.z + t->pos.z * CHUNK_SIZE;

          double val = p.noise(pos.x / (double) 32, pos.z / (double) 32, 0) * 16;
          val *= p2.noise((pos.x + 16) / (double) 48, (pos.z + 16) / (double) 48, 0) * 8;
          val /= 16;
          val *= pow(p.noise(pos.x / (double) 64, pos.z / (double) 64, 0), 2) * 40 + 1;
          val -= pos.y;

          int block = (val > 0) ? (val > 1 ) ? (val > 4) ? 3 : 2 : 1 : 0;
          blocks->push_back(block);
     }

     t->chunk = new BlockChunk(blocks);
     t->done = true;
}

void meshGenThread(World::MeshThreadData *t) {
    MeshGenerator().build(t->chunk, t->atlas, *(t->vertices), *(t->indices));
    t->done = true;
}

void World::handleChunkGenQueue() {
    //Finalize finished threads by added the BlockChunks to the world via newChunk().
    for (auto iter = chunkGenThreads.begin(); iter != chunkGenThreads.end(); ) {
        ChunkThreadData* threadData = (*iter);

        if (threadData->done) {
            newChunk(threadData->pos, threadData->chunk);
            delete threadData;
            iter = chunkGenThreads.erase(iter);
        }
        else {
            iter++;
        }
    }

    //Create new chunk gen threads if there are chunks to be generated.
    while (!chunkGenQueue.empty() && chunkGenThreads.size() < MAX_CHUNK_GEN_THREADS) {
        auto it = chunkGenQueue.begin();
        chunkGenQueue.erase(chunkGenQueue.begin());
        glm::vec3 pos = (*it);

        auto t = new ChunkThreadData(pos, blockAtlas);
        auto thread = new std::thread(chunkGenThread, t);
        thread->detach();
        t->thread = thread;
        chunkGenThreads.push_back(t);
    }
}

void World::handleMeshGenQueue() {
    //Finalize finished threads by creating a MeshChunk object from the thread data.
    for (auto iter = meshGenThreads.begin(); iter != meshGenThreads.end(); ) {
        MeshThreadData* threadData = (*iter);

        if (threadData->done) {
            //Only create the MeshChunk object if there's vertices in it.
            if (!threadData->vertices->empty()) {
                auto meshChunk = new MeshChunk();
                meshChunk->build(threadData->vertices, threadData->indices);

                glm::vec3 pos = threadData->pos * glm::vec3(CHUNK_SIZE);
                meshChunk->setPosition(pos);
                meshChunks.insert(std::pair<glm::vec3, MeshChunk *>(threadData->pos, meshChunk));
            }

            delete threadData;
            iter = meshGenThreads.erase(iter);
        }
        else {
            iter++;
        }
    }

    //Create new mesh gen threads if there are meshes to be made.
    while (!meshGenQueue.empty() && meshGenThreads.size() < MAX_MESH_GEN_THREADS) {
        auto it = meshGenQueue.begin();
        meshGenQueue.erase(meshGenQueue.begin());
        glm::vec3 pos = (*it);

        auto t = new MeshThreadData(pos, blockChunks.at(pos), blockAtlas);
        auto thread = new std::thread(meshGenThread, t);
        thread->detach();
        t->thread = thread;
        meshGenThreads.push_back(t);
    }
}

void World::update() {
    //Create / Finalize BlockChunks
    handleChunkGenQueue();

    //Create / Finalize MeshChunks
    handleMeshGenQueue();
}

std::unordered_map<glm::vec3, MeshChunk*, World::vec3cmp>* World::getMeshChunks() {
    return &meshChunks;
}

//
//Constructors and Destructors for the MeshThreadData / ChunkThreadData structs.
//
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
    //Delete the thread, when the deconstructor is called, the thread is done.
    delete thread;
}

World::ChunkThreadData::ChunkThreadData(glm::vec3 pos, BlockAtlas *atlas) {
    this->pos = pos;
    this->atlas = atlas;
    this->done = false;
    this->chunk = nullptr;
}

World::ChunkThreadData::~ChunkThreadData() {
    //Delete the thread, when the deconstructor is called, the thread is done.
    delete thread;
}