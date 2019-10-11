//
// Created by aurailus on 04/04/19.
//

#include "LocalDimension.h"

LocalDimension::LocalDimension(LocalDefs &defs) : meshGenStream(std::make_unique<MeshGenStream>(defs, *this)) {}

void LocalDimension::update(double delta, glm::vec3 playerPos) {
    finishMeshes();
    queueMeshes();

    for (auto& entities : luaEntities) {
        entities->entity->update(delta);
    }

    auto chunkPosOfPlayer = TransPos::chunkFromVec(playerPos);

    //TODO: Figure out why there are NULL CHUNKS in the map
    for (auto it = blockChunks.begin(); it != blockChunks.end();) {
        auto pos = it->first;

        if (it->second != nullptr) {
            auto diffVec = pos - chunkPosOfPlayer;
            float distance = max(abs(diffVec.x), max(abs(diffVec.y), abs(diffVec.z)));

            //TODO: Don't hard code this number
            if (distance >= 24) {
                removeMeshChunk(pos);
                it = blockChunks.erase(it);
            } else {
                it++;
            }
        }
        else {
            std::cout << Log::err << "Deleted Null Chunk in LocalDimension." << Log::endl;
            it = blockChunks.erase(it);
        }
    }
}

void LocalDimension::finishMeshes() {
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

void LocalDimension::queueMeshes() {
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

int LocalDimension::renderChunks(Renderer &renderer) {
    int count = 0;
    for (auto &renderElement : renderElems) {
        FrustumAABB bbox(renderElement->getPos() * glm::vec3(TransPos::CHUNK_SIZE), glm::vec3(TransPos::CHUNK_SIZE));
        if (renderer.getCamera().inFrustum(bbox) != Frustum::OUTSIDE) {
            renderElement->draw(renderer);
            count++;
        }
    }
    return count;
}

void LocalDimension::renderEntities(Renderer &renderer) {
    for (auto& entity : luaEntities) {
        entity->entity->draw(renderer);
    }
}

void LocalDimension::setChunk(sptr<BlockChunk> chunk) {
    blockChunks.insert({chunk->pos, chunk});
    attemptMeshChunk(chunk);
}

std::shared_ptr<BlockChunk> LocalDimension::getChunk(glm::vec3 pos) {
    if (blockChunks.count(pos)) return blockChunks.at(pos);
    return nullptr;
}

void LocalDimension::setMeshChunk(std::shared_ptr<MeshChunk> meshChunk) {
    if (renderRefs.count(meshChunk->getPos())) removeMeshChunk(meshChunk->getPos());
    renderElems.push_back(static_pointer_cast<ChunkRenderElem>(meshChunk));
    renderRefs.emplace(meshChunk->getPos(), --renderElems.end());
}

void LocalDimension::removeMeshChunk(const glm::vec3& pos) {
    if (!renderRefs.count(pos)) return;
    auto refIter = renderRefs.at(pos);

    if (!refIter->get()->updateChunkUse(pos, false)) {
        renderElems.erase(refIter);
        renderRefs.erase(pos);
    }
}

void LocalDimension::addLuaEntity(sptr<LuaEntity> &entity) {
    luaEntities.push_back(entity);
    luaEntityRefs.emplace(entity->id, --luaEntities.end());
}

void LocalDimension::removeLuaEntity(sptr<LuaEntity> &entity) {
    if (!luaEntityRefs.count(entity->id)) return;
    auto refIter = luaEntityRefs.at(entity->id);

    luaEntities.erase(refIter);
    luaEntityRefs.erase(entity->id);
}

int LocalDimension::getMeshChunkCount() {
    return static_cast<int>(renderElems.size());
}

void LocalDimension::setBlock(glm::vec3 pos, unsigned int block) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) chunk->setBlock(local, block);
    chunk->dirty = true;

    attemptMeshChunk(chunk);
}

unsigned int LocalDimension::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBlock(local);
    return 0;
}

void LocalDimension::attemptMeshChunk(const sptr<BlockChunk>& chunk, bool updateAdjacents) {
//    if (!chunk->dirty) return; //TODO

    auto dirs = VecUtils::getCardinalVectors();
    bool allExists = true;
    for (auto dir : dirs) {
        if (!getAdjacentExists(chunk->pos + dir, updateAdjacents)) {
            allExists = false;
        }
    }

    if (allExists && chunk->shouldRender()) {
        chunk->dirty = false;
        pendingMesh.push_back(chunk->pos);
    }
}

bool LocalDimension::getAdjacentExists(glm::vec3 pos, bool updateAdjacents) {
    auto chunk = getChunk(pos);
    if (chunk == nullptr) return false;
    if (updateAdjacents) attemptMeshChunk(chunk, false);
    return true;
}