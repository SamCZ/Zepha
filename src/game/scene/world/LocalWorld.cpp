//
// Created by aurailus on 14/12/18.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "LocalWorld.h"
#include "../../../util/Vec.h"

LocalWorld::LocalWorld(GameDefs& defs) :
    meshGenStream(defs, dimension),
    worldGenStream(55),
    defs(defs) {}

void LocalWorld::loadChunkPacket(Packet *p) {
    worldGenStream.pushBack(p);
}

void LocalWorld::commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk> c) {
    dimension.addChunk(pos, std::move(c));
    attemptMeshChunk(pos);
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

void LocalWorld::update(glm::vec3 playerChunkPos) {
    //Create Finished Messages
    auto finishedMeshes = meshGenStream.update();

    lastMeshUpdates = 0;
    for (auto mesh : *finishedMeshes) {
        if (!mesh.vertices->empty()) {
            auto meshChunk = new MeshChunk();
            meshChunk->build(mesh.vertices, mesh.indices);
            meshChunk->setPos(mesh.pos);

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
                moreSpace = meshGenStream.tryToQueue(pos);
            }

            pendingMesh.erase(it);
        }
    }

    //Create Finished BlockChunks
    auto finishedChunks = worldGenStream.update();

    lastGenUpdates = 0;
    for (const auto &chunk : finishedChunks) {
        commitChunk(chunk->pos, chunk);
        lastGenUpdates++;
    }

    //Delete far-out regions
//    int toDelete = 0;
//    for (const auto &region : dimension.getRegions()) {
//        glm::vec3 middlePos = region.second->getRawPos() + glm::vec3(TransPos::REGION_CHUNK_LENGTH / 2);
//        float dist = glm::distance(middlePos, playerChunkPos);
//        if (dist > 64) toDelete++;
//    }
//    std::cout << toDelete << ", " << dimension.getRegions().size() << std::endl;
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
    return defs.blocks().getBlock(blockId).isSolid();
}

std::shared_ptr<BlockChunk> LocalWorld::getChunk(glm::vec3 chunkPos) {
    return dimension.getChunk(chunkPos);
}

std::unordered_map<glm::vec3, MeshChunk*, VecUtils::compareFunc>* LocalWorld::getMeshChunks() {
    return &meshChunks;
}

int LocalWorld::render(Renderer &renderer, glm::vec3 playerChunkPos) {
    int count = 0;

    for (auto &chunkPair : meshChunks) {
        auto chunk = chunkPair.second;

        auto diffVec = chunkPair.first - playerChunkPos;
        float distance = max(abs(diffVec.x), max(abs(diffVec.y), abs(diffVec.z)));

        if (distance < 8) {
            FrustumAABB bbox(chunk->getPos(), glm::vec3(TransPos::CHUNK_SIZE));

            if (renderer.getCamera()->inFrustum(bbox) != Frustum::OUTSIDE) {
                chunk->draw(renderer);
                count++;
            }
        }
    }

    return count;
}
