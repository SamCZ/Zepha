//
// Created by aurailus on 22/01/19.
//

#include "WorldThreadDefs.h"
#include "World.h"

ChunkThreadData::ChunkThreadData(glm::vec3 pos, BlockAtlas *atlas) {
    this->pos = pos;
    this->atlas = atlas;
    this->done = false;
    this->chunk = nullptr;
}

ChunkThreadDef::ChunkThreadDef(MapGen *mapGen) {
    this->mapGen = mapGen;

    thread = new std::thread(World::chunkGenThread, this);

    sched_param sch_params{};
    sch_params.sched_priority = 1;
    pthread_setschedparam(thread->native_handle(), SCHED_RR, &sch_params);

    thread->detach();
}

ChunkThreadDef::~ChunkThreadDef() {
    delete thread;
}

MeshThreadData::MeshThreadData(glm::vec3 pos, BlockChunk *chunk, std::vector<bool>* adjacents, BlockAtlas *atlas) {
    this->pos = pos;
    this->chunk = chunk;
    this->atlas = atlas;
    this->adjacents = adjacents;
    this->done = false;
    this->vertices = new std::vector<float>();
    this->indices = new std::vector<unsigned int>();
}

MeshThreadData::~MeshThreadData() {
    delete vertices;
    delete indices;
    delete adjacents;
}

MeshThreadDef::MeshThreadDef() {
    thread = new std::thread(World::meshGenThread, this);

    sched_param sch_params{};
    sch_params.sched_priority = 1;
    pthread_setschedparam(thread->native_handle(), SCHED_RR, &sch_params);

    thread->detach();
}

MeshThreadDef::~MeshThreadDef() {
    delete thread;
}