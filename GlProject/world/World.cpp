//
// Created by aurailus on 14/12/18.
//

#include "World.h"

World::World() {
    blockAtlas = nullptr;
}

World::World(BlockAtlas *atlas) {
    blockAtlas = atlas;
}

void World::newChunk(glm::vec3* pos, BlockChunk *c) {
    blockChunks.insert(std::pair<glm::vec3*, BlockChunk*>(pos, c));
    meshGenQueue.insert(pos);
}

void genThread(World::ThreadData* t) {
    MeshGenerator().build(t->chunk, t->atlas, *(t->vertices), *(t->indices));
    t->done = true;
}

void World::update() {
    //Run through all of the active generation threads, and finish and remove the ones that are ready to be finished.
    //Then, spin up new threads when there is space in the genThreads array and there are chunks waiting to be meshed.

    //Create MeshChunks for the finished threads.
    Timer applyGlobal("Applying Meshes");
    for (auto iter = genThreads.begin(); iter != genThreads.end(); ) {
        ThreadData* threadData = (*iter);

        //If the threadData is done, create a MeshChunk and delete the threadData.
        if (threadData->done) {
            Timer apply("Applying Mesh");

            auto meshChunk = new MeshChunk();
            meshChunk->build(threadData->vertices, threadData->indices);
            meshChunk->setPosition(*(threadData->pos) * glm::vec3(CHUNK_SIZE));

            meshChunks.insert(std::pair<glm::vec3*, MeshChunk*>(threadData->pos, meshChunk));

//            apply.elapsedMs();

            delete threadData;
            iter = genThreads.erase(iter);
        }
        //Otherwise ignore it and move to the next MeshChunk
        else {
            iter++;
        }
    }
//    applyGlobal.elapsedMs();

    //Begin processing queued chunks if there are chunks to be processed and there's room for more threads.
    while (!meshGenQueue.empty() && genThreads.size() < MAX_CONCURRENT_THREADS) {

        //Get and remove the first position from the vector.
        auto it = meshGenQueue.begin();
        meshGenQueue.erase(meshGenQueue.begin());
        glm::vec3* pos = (*it);

        //Create a thread for it and add the threadData to the genThreads vector
        auto t = new ThreadData(pos, blockChunks.at(pos), blockAtlas);
        auto thread = new std::thread(genThread, t);
        thread->detach();
        t->thread = thread;
        genThreads.push_back(t);
    }
}

void World::draw(GLint modelUni) {
    for (auto &meshChunk : meshChunks) {
        glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(meshChunk.second->getModelMatrix()));
        meshChunk.second->draw();
    }
}

World::ThreadData::ThreadData(glm::vec3 *pos, BlockChunk *chunk, BlockAtlas *atlas) {
    this->pos = pos;
    this->chunk = chunk;
    this->atlas = atlas;
    this->done = false;
    this->vertices = new std::vector<float>();
    this->indices = new std::vector<unsigned int>();
}

World::ThreadData::~ThreadData() {
    //Not delete this->pos because it is used in the MeshChunk and BlockChunk vectors.
    //delete pos
    delete vertices;
    delete indices;
    //Delete the thread, when the deconstructor is called, the thread is done.
    delete thread;
}