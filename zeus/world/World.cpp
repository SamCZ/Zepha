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

void World::genChunk(glm::vec3* pos) {
    chunkGenQueue.insert(pos);
}

void World::newChunk(glm::vec3* pos, BlockChunk *c) {
    blockChunks.insert(std::pair<glm::vec3*, BlockChunk*>(pos, c));
    meshGenQueue.insert(pos);
}

void chunkGenThread(World::ChunkThreadData *t) {
    PerlinNoise p(0);

    auto* blocks = new std::vector<int>();
    blocks->reserve(4096);

     glm::vec3 innerPos, pos;

     for (int ind = 0; ind < 4096; ind++) {
          ArrayTrans3D::indAssignVec(ind, &innerPos);

          pos.x = innerPos.x + t->pos->x * CHUNK_SIZE;
          pos.y = innerPos.y + t->pos->y * CHUNK_SIZE;
          pos.z = innerPos.z + t->pos->z * CHUNK_SIZE;

          double val = p.noise(pos.x / (double) 32, pos.z / (double) 32, 0) * 16 - pos.y;

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

void World::update() {
    int done = 0;
    //Finish Block Gen threads
    for (auto iter = chunkGenThreads.begin(); iter != chunkGenThreads.end(); ) {
        ChunkThreadData* threadData = (*iter);

        //If the threadData is done, create a BlockChunk and delete the threadData.
        if (threadData->done) {
            done++;
//            blockChunks.insert(std::pair<glm::vec3*, BlockChunk*>(threadData->pos, threadData->chunk));
            newChunk(threadData->pos, threadData->chunk);

            delete threadData;

            iter = chunkGenThreads.erase(iter);
        }
        //Otherwise ignore it and move to the next BlockChunk
        else {
            iter++;
        }
    }

    //Create threads for the chunk generation.
    while (!chunkGenQueue.empty() && chunkGenThreads.size() < MAX_CHUNK_GEN_THREADS) {
        //Get and remove the first position from the vector.
        auto it = chunkGenQueue.begin();
        chunkGenQueue.erase(chunkGenQueue.begin());
        glm::vec3* pos = (*it);

        //Create a thread for it and add the threadData to the chunkGenThreads vector
        auto t = new ChunkThreadData(pos, blockAtlas);
        auto thread = new std::thread(chunkGenThread, t);
        thread->detach();
        t->thread = thread;
        chunkGenThreads.push_back(t);
    }

    //Run through all of the active generation threads, and finish and remove the ones that are ready to be finished.
    //Then, spin up new threads when there is space in the meshGenThreads array and there are chunks waiting to be meshed.

    //Create MeshChunks for the finished threads.
    Timer applyGlobal("Applying Meshes");
    for (auto iter = meshGenThreads.begin(); iter != meshGenThreads.end(); ) {
        MeshThreadData* threadData = (*iter);

        //If the threadData is done, create a MeshChunk and delete the threadData.
        if (threadData->done) {
            Timer apply("Applying Mesh");

            auto meshChunk = new MeshChunk();
            meshChunk->build(threadData->vertices, threadData->indices);
            meshChunk->setPosition(*(threadData->pos) * glm::vec3(CHUNK_SIZE));

            meshChunks.insert(std::pair<glm::vec3*, MeshChunk*>(threadData->pos, meshChunk));

//            apply.elapsedMs();

            delete threadData;
            iter = meshGenThreads.erase(iter);
        }
        //Otherwise ignore it and move to the next MeshChunk
        else {
            iter++;
        }
    }

//    applyGlobal.elapsedMs();

    //Begin processing queued chunks if there are chunks to be processed and there's room for more threads.
    while (!meshGenQueue.empty() && meshGenThreads.size() < MAX_MESH_GEN_THREADS) {

        //Get and remove the first position from the vector.
        auto it = meshGenQueue.begin();
        meshGenQueue.erase(meshGenQueue.begin());
        glm::vec3* pos = (*it);

        //Create a thread for it and add the threadData to the meshGenThreads vector
        auto t = new MeshThreadData(pos, blockChunks.at(pos), blockAtlas);
        auto thread = new std::thread(meshGenThread, t);
        thread->detach();
        t->thread = thread;
        meshGenThreads.push_back(t);
    }
}

std::map<glm::vec3*, MeshChunk*>* World::getMeshChunks() {
    return &meshChunks;
}

World::MeshThreadData::MeshThreadData(glm::vec3 *pos, BlockChunk *chunk, BlockAtlas *atlas) {
    this->pos = pos;
    this->chunk = chunk;
    this->atlas = atlas;
    this->done = false;
    this->vertices = new std::vector<float>();
    this->indices = new std::vector<unsigned int>();
}

World::MeshThreadData::~MeshThreadData() {
    //Not delete this->pos because it is used in the MeshChunk and BlockChunk vectors.
    //delete pos
    delete vertices;
    delete indices;
    //Delete the thread, when the deconstructor is called, the thread is done.
    delete thread;
}

World::ChunkThreadData::ChunkThreadData(glm::vec3 *pos, BlockAtlas *atlas) {
    this->pos = pos;
    this->atlas = atlas;
    this->done = false;
    this->chunk = nullptr;
}

World::ChunkThreadData::~ChunkThreadData() {
    //Delete the thread, when the deconstructor is called, the thread is done.
    delete thread;
}