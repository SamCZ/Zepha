//
// Created by aurailus on 14/12/18.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "LocalWorld.h"
#include "../../../generic/helpers/VecUtils.h"

LocalWorld::LocalWorld(BlockAtlas *atlas) :
    meshGenStream(atlas),
    worldGenStream(55) {

    blockAtlas = atlas;
}

void LocalWorld::loadChunkPacket(Packet *p) {
    worldGenStream.pushBack(p);
}

void LocalWorld::commitChunk(glm::vec3 pos, BlockChunk *c) {
//    dimension.addChunk(pos, c);
//    attemptMeshChunk(pos);

    if (!blockChunks.count(pos)) {
        blockChunks.insert(std::pair<glm::vec3, BlockChunk *>(pos, c));

        attemptMeshChunk(pos);
    }
}

void LocalWorld::remeshChunk(glm::vec3 pos) {
    attemptMeshChunk(pos);
}

void LocalWorld::attemptMeshChunk(glm::vec3 pos) {
    auto thisChunk = getChunk(pos);
    if (thisChunk == nullptr) return;

    auto vectors = VecUtils::getCardinalVectors();
    for (int i = 0; i < vectors.size(); i++) {
        thisChunk->adjacent[i] = getAdjacentExists(pos + vectors[i], pos);
    }

    if (thisChunk->allAdjacentsExist() && !thisChunk->isEmpty()) pendingMesh.push_back(pos);
}

bool LocalWorld::getAdjacentExists(glm::vec3 pos, glm::vec3 otherPos) {
    auto chunk = getChunk(pos);
    glm::vec3 diff = otherPos - pos;

    if (chunk != nullptr) {
        auto vectors = VecUtils::getCardinalVectors();
        for (int i = 0; i < vectors.size(); i++) {
            if (diff == vectors[i]) {
                chunk->adjacent[i] = true;
            }
        }

        if (chunk->allAdjacentsExist() && !chunk->isEmpty()) pendingMesh.push_back(pos);
        return true;
    }
    return false;
}

std::vector<bool>* LocalWorld::getAdjacentsCull(glm::vec3 pos) {
    auto culls = new std::vector<bool>();
    culls->reserve(1536); //256 * 6

    auto vectors = VecUtils::getCardinalVectors();
    for (int i = 0; i < vectors.size(); i++) {
        auto chunk = getChunk(pos + vectors[i]);

        for (int j = 0; j < 16; j++) {
            for (int k = 0; k < 16; k++) {

                int x = (i == 0) ? 0 : (i == 1) ? 15 : (i <= 3) ? j : k;
                int y = (i == 2) ? 0 : (i == 3) ? 15 : j;
                int z = (i == 4) ? 0 : (i == 5) ? 15 : k;

                auto block = chunk->getBlock(x, y, z);
                culls->push_back(blockAtlas->getBlock(block)->isCulling());
            }
        }
    }
    return culls;
}

void LocalWorld::update() {
    //Create Finished Messages
    auto finishedMeshes = meshGenStream.update();

    lastMeshUpdates = 0;
    for (auto mesh : *finishedMeshes) {
        if (!mesh.vertices->empty()) {
            auto meshChunk = new MeshChunk();
            meshChunk->build(mesh.vertices, mesh.indices);

            glm::vec3 pos = mesh.pos * glm::vec3(CHUNK_SIZE);
            meshChunk->setPosition(pos);

            if (meshChunks.count(mesh.pos)) {
                MeshChunk* oldChunk = meshChunks.at(mesh.pos);
                meshChunks.erase(mesh.pos);
                delete oldChunk;
            }
            meshChunks.insert(std::pair<glm::vec3, MeshChunk*>(mesh.pos, meshChunk));

            lastMeshUpdates++;
        }
    }

    //Build New Meshes
    if (meshGenStream.spaceInQueue()) {
        bool moreSpace = true;

        while (moreSpace && !pendingMesh.empty()) {
            auto it = pendingMesh.begin();
            glm::vec3 pos = *it;

            if (!meshGenStream.isQueued(pos)) {
                moreSpace = meshGenStream.tryToQueue(
                        std::pair<BlockChunk*, std::vector<bool>*>{getChunk(pos), getAdjacentsCull(pos)});
            }

            pendingMesh.erase(it);
        }
    }

    //Create Finished BlockChunks
    auto finishedChunks = worldGenStream.update();

    lastGenUpdates = 0;
    for (auto chunk : finishedChunks) {
        commitChunk(chunk->pos, chunk);
        lastGenUpdates++;
    }
}

int LocalWorld::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) {
        return chunk->getBlock(&local);
    }
    return -1;
}

void LocalWorld::setBlock(glm::vec3 pos, int block) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) {
        if (chunk->setBlock(&local, block)) {
            remeshChunk(chunkPos);
        }
    }
}

bool LocalWorld::solidAt(glm::vec3 pos) {
    int blockId = getBlock(pos);
    if (blockId == -1) return true;
    return blockAtlas->getBlock(blockId)->isSolid();
}

BlockChunk* LocalWorld::getChunk(glm::vec3 chunkPos) {
    if (blockChunks.count(chunkPos) == 1) {
        return blockChunks[chunkPos];
    }
    return nullptr;
}

std::unordered_map<glm::vec3, MeshChunk*, VecUtils::compareFunc>* LocalWorld::getMeshChunks() {
    return &meshChunks;
}
