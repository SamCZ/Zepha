//
// Created by aurailus on 04/04/19.
//

#include "Dimension.h"

Dimension::Dimension(LocalDefs &defs) : meshGenStream(std::make_unique<MeshGenStream>(defs, *this)) {}

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
    //TODO: Figure out why there are NULL CHUNK in the map
    for (auto it = blockChunks.begin(); it != blockChunks.end();) {
        auto chunk = it->second;
        auto pos = it->first;

        if (chunk != nullptr) {
            auto diffVec = pos - *playerPos;
            float distance = max(abs(diffVec.x), max(abs(diffVec.y), abs(diffVec.z)));

            //TODO: Don't hard code this number
            if (distance >= 24) {
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
            std::cout << Log::err << "Deleted Null Chunk in Dimension." << Log::endl;
            it = blockChunks.erase(it);
        }
    }
}

void Dimension::finishMeshes() {
    lastMeshUpdates = 0;
    auto finishedMeshes = meshGenStream->update();

    for (MeshDetails* meshDetails : finishedMeshes) {

        if (!meshDetails->vertices.empty()) {
            auto meshChunk = std::make_shared<MeshChunk>();
            meshChunk->create(meshDetails->vertices, meshDetails->indices);
            meshChunk->setPos(meshDetails->pos);

            setMeshChunk(meshChunk);
            lastMeshUpdates++;
        } else {
            removeMeshChunk(meshDetails->pos);
        }

        delete meshDetails;
    }
}

void Dimension::queueMeshes() {
    if (meshGenStream->spaceInQueue()) {
        bool moreSpace = true;

        while (moreSpace && !pendingMesh.empty()) {
            auto it = pendingMesh.begin();
            glm::vec3 pos = *it;

            if (!meshGenStream->isQueued(pos)) {
                moreSpace = meshGenStream->tryToQueue(pos);
            }

            pendingMesh.erase(it);
        }
    }
}

int Dimension::render(Renderer &renderer) {
    int count = 0;

    for (auto &chunk : meshChunks) {
        FrustumAABB bbox(chunk->getPos() * glm::vec3(TransPos::CHUNK_SIZE), glm::vec3(TransPos::CHUNK_SIZE));

        if (renderer.getCamera().inFrustum(bbox) != Frustum::OUTSIDE) {
            chunk->draw(renderer);
            count++;
        }
    }

    return count;
}

int Dimension::getMeshChunkCount() {
    return (int)meshChunks.size();
}

void Dimension::setBlock(glm::vec3 pos, unsigned int block) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) chunk->setBlock(local, block);
    chunk->dirty = true;

    auto dirs = VecUtils::getCardinalVectors();
    attemptMeshChunk(chunk);
}

unsigned int Dimension::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBlock(local);
    return 0;
}

void Dimension::removeMeshChunk(const glm::vec3& pos) {
    std::shared_ptr<BlockChunk> blockChunk = blockChunks[pos];

    if (blockChunk != nullptr && blockChunk->meshChunk != nullptr) {
        meshChunks.erase(blockChunk->meshChunkIter);
        delete blockChunk->meshChunk;
        blockChunk->meshChunk = nullptr;
    }

}
