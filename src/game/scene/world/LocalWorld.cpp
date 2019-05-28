//
// Created by aurailus on 14/12/18.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "LocalWorld.h"

LocalWorld::LocalWorld(GameDefs& defs, glm::vec3* playerPos) :
    playerPos(playerPos),
    dimension(&playerChunkPos),
    meshGenStream(defs, dimension),
    worldGenStream(55),
    defs(defs) {}

void LocalWorld::update(double delta) {
    playerChunkPos = TransPos::roundPos(*playerPos / glm::vec3(TransPos::CHUNK_SIZE));

    finishMeshes();
    queueMeshes();
    finishChunks();

    updateBlockDamages(delta);
    dimension.update();

    auto end = particles.begin();
    for (auto i = particles.begin(); i < particles.end(); i++) {
        (*i)->update(delta, *playerPos);
        if ((*i)->time > 1) {
            end = i;
            delete (*i);
        }
    }
    if (end != particles.begin()) particles.erase(particles.begin(), end + 1);
}

void LocalWorld::damageBlock(glm::vec3 pos, float amount) {
    BlockCrackEntity* block = nullptr;
    for (auto test : crackedBlocks) {
        if (test->blockPos == pos) {
            block = test;
            break;
        }
    }
    if (block == nullptr) {
        auto blockID = static_cast<unsigned int>(getBlock(pos));
        block = new BlockCrackEntity(defs, pos, blockID);
        crackedBlocks.push_back(block);
    }

    block->setNewDamage(block->targetDamage + amount);
    block->time = 0;

    auto def = defs.blocks().getBlock(getBlock(pos));
    for (int i = 0; i < 40 * amount; i++) {
        auto p = new ParticleEntity(pos, def);
        particles.push_back(p);
    }
}

void LocalWorld::finishMeshes() {
    lastMeshUpdates = 0;
    auto finishedMeshes = meshGenStream.update();

    for (auto mesh : finishedMeshes) {
        if (!mesh.vertices->empty()) {
            auto meshChunk = new MeshChunk();
            meshChunk->build(*mesh.vertices, *mesh.indices);
            meshChunk->setPos(mesh.pos);

            if (meshChunks.count(mesh.pos)) {
                MeshChunk* oldChunk = meshChunks.at(mesh.pos);
                meshChunks.erase(mesh.pos);
                delete oldChunk;
            }
            dimension.addMeshChunk(meshChunk);
            lastMeshUpdates++;
        }
    }
}

void LocalWorld::queueMeshes() {
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
}
void LocalWorld::finishChunks() {
    auto finishedChunks = worldGenStream.update();

    lastGenUpdates = 0;
    for (const auto &chunk : finishedChunks) {
        commitChunk(chunk->pos, chunk);
        lastGenUpdates++;
    }
}
void LocalWorld::updateBlockDamages(double delta) {
    auto it = crackedBlocks.cbegin();
    while (it != crackedBlocks.cend()) {
        bool deleteMe = false;

        auto curr = it++;
        auto block = *curr;

        block->time += delta;

        if (block->damage >= 1) {
            setBlock(block->blockPos, 0);
            deleteMe = true;
        }

        if (block->time > 2) {
            block->update();
            block->setNewDamage(block->targetDamage - 0.1f);
            block->time = 0;
        }

        if (block->damage < 0 || block->blockID != getBlock(block->blockPos)) {
            deleteMe = true;
        }

        if (deleteMe) {
            delete *curr;
            it = crackedBlocks.erase(curr);
        }
        else {
            block->update();
        }
    }
}


void LocalWorld::loadChunkPacket(Packet p) {
    worldGenStream.pushBack(p);
}
std::shared_ptr<BlockChunk> LocalWorld::getChunk(glm::vec3 chunkPos) {
    return dimension.getChunk(chunkPos);
}
void LocalWorld::commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk> c) {
    dimension.addBlockChunk(std::move(c));
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

int LocalWorld::render(Renderer &renderer) {
    auto count = dimension.render(renderer);

    for (auto block : crackedBlocks) block->draw(renderer);
    for (auto &p : particles) p->draw(renderer);

    return count;
}
int LocalWorld::getMeshChunkCount() {
    return dimension.getMeshChunkCount();
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