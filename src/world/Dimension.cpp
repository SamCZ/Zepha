//
// Created by aurailus on 04/04/19.
//

#include "Dimension.h"

Dimension::Dimension(glm::vec3 *playerPos) {
    this->playerPos = playerPos;
}

void Dimension::addBlockChunk(std::shared_ptr<BlockChunk> chunk) {
    if (blockChunks.count(chunk->pos)) {
        auto oldChunk = blockChunks[chunk->pos];
        chunk->meshChunk = oldChunk->meshChunk;
        chunk->meshChunkIter = oldChunk->meshChunkIter;
        blockChunks.erase(chunk->pos);
    }
    blockChunks.insert({chunk->pos, chunk});
}

void Dimension::addMeshChunk(MeshChunk* meshChunk) {
    auto blockChunk = blockChunks[meshChunk->getPos()];

    if (!blockChunk) {
        blockChunk = std::make_shared<BlockChunk>();
        addBlockChunk(blockChunk);
    }
    else if (blockChunk->meshChunk != nullptr) {
        meshChunks.erase(blockChunk->meshChunkIter);
        delete blockChunk->meshChunk;
    }

    blockChunk->meshChunk = meshChunk;
    meshChunks.push_back(meshChunk);
    blockChunk->meshChunkIter = (std::list<MeshChunk*>::iterator) --meshChunks.end();
}

std::shared_ptr<BlockChunk> Dimension::getChunk(glm::vec3 pos) {
    if (blockChunks.count(pos)) {
        return blockChunks.at(pos);
    }
    return nullptr;
}

Dimension::~Dimension() {
    for (auto chunk : blockChunks) {
        delete chunk.second->meshChunk;
    }
}

void Dimension::update() {
    //TODO: Figure out why there are NULL CHUNKS in the map
    for (auto it = blockChunks.begin(); it != blockChunks.end();) {
        auto chunk = it->second;
        auto pos = it->first;

        if (chunk != nullptr) {
            auto diffVec = pos - *playerPos;
            float distance = max(abs(diffVec.x), max(abs(diffVec.y), abs(diffVec.z)));

            if (distance > 16) {
                if (chunk->meshChunk != nullptr) {
                    meshChunks.erase(chunk->meshChunkIter);
                    delete chunk->meshChunk;
                }
                it = blockChunks.erase(it);
            } else {
                it++;
            }
        }
        else {
            std::cerr << "null chunk" << std::endl;
            it = blockChunks.erase(it);
        }
    }
}

int Dimension::render(Renderer &renderer) {
    int count = 0;

    for (auto &chunk : meshChunks) {
        FrustumAABB bbox(chunk->getPos() * glm::vec3(TransPos::CHUNK_SIZE), glm::vec3(TransPos::CHUNK_SIZE));

        if (renderer.getCamera()->inFrustum(bbox) != Frustum::OUTSIDE) {
            chunk->draw(renderer);
            count++;
        }
    }

    return count;
}

int Dimension::getMeshChunkCount() {
    return (int)meshChunks.size();
}