//
// Created by aurailus on 14/12/18.
//

#include "World.h"

World::World(BlockAtlas *atlas) {
    blockAtlas = atlas;
}

void World::newChunk(glm::vec3* pos, BlockChunk *c) {
    blockChunks.insert(std::pair<glm::vec3*, BlockChunk*>(pos, c));
    meshGenQueue.insert(pos);
}

void World::update() {
    int done = 0;
    while (!meshGenQueue.empty() && done < 8) {
        auto it = meshGenQueue.begin();
        meshGenQueue.erase(meshGenQueue.begin());
        glm::vec3* pos = (*it);

        auto meshChunk = new MeshChunk(blockChunks.at(pos));
        meshChunk->build(blockAtlas);
        meshChunk->setPosition(glm::vec3(pos->x * CHUNK_SIZE, pos->y * CHUNK_SIZE, pos->z * CHUNK_SIZE));

        meshChunks.insert(std::pair<glm::vec3*, MeshChunk*>(pos, meshChunk));
    }
}

void World::draw(GLint modelUni) {
    for (auto &meshChunk : meshChunks) {
        glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(meshChunk.second->getModelMatrix()));
        meshChunk.second->draw();
    }
}