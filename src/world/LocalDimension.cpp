//
// Created by aurailus on 04/04/19.
//

#include "LocalDimension.h"

LocalDimension::LocalDimension(LocalDefs &defs) : defs(defs), meshGenStream(std::make_unique<MeshGenStream>(defs, *this)) {}

void LocalDimension::update(double delta, glm::vec3 playerPos) {
    finishMeshes();
    queueMeshes();

    for (auto& entities : localEntities) entities->entity->update(delta);
    for (auto& entities : serverEntities) entities->entity->update(delta);
    for (auto& playerEnt : playerEntities) playerEnt.update(delta);

    auto chunkPosOfPlayer = Space::Chunk::world::fromBlock(playerPos);

//    for (auto it = blockChunks.begin(); it != blockChunks.end();) {
//        auto pos = it->first;
//
//        auto diffVec = pos - chunkPosOfPlayer;
//        float distance = max(abs(diffVec.x), max(abs(diffVec.y), abs(diffVec.z)));
//
//        //TODO: Don't hard code this number
//        if (distance >= 24) {
//            removeMeshChunk(pos);
//            it = blockChunks.erase(it);
//        } else {
//            it++;
//        }
//    }
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
        FrustumAABB bbox(renderElement->getPos() * glm::vec3(16), glm::vec3(16));
        if (renderer.camera.inFrustum(bbox) != Frustum::OUTSIDE) {
            renderElement->draw(renderer);
            count++;
        }
    }
    return count;
}

void LocalDimension::renderEntities(Renderer &renderer) {
    for (auto& entity : localEntities) entity->entity->draw(renderer);
    for (auto& entity : serverEntities) entity->entity->draw(renderer);
    for (auto& entity : playerEntities) entity.draw(renderer);
}

void LocalDimension::setChunk(sptr<BlockChunk> chunk) {
    Dimension::setChunk(chunk);
    attemptMeshChunk(chunk);
}

void LocalDimension::setMeshChunk(std::shared_ptr<MeshChunk> meshChunk) {
    if (renderRefs.count(meshChunk->getPos())) removeMeshChunk(meshChunk->getPos());
    renderElems.push_back(std::static_pointer_cast<ChunkRenderElem>(meshChunk));
    renderRefs.emplace(meshChunk->getPos(), --renderElems.end());
}

void LocalDimension::removeMeshChunk(const glm::ivec3& pos) {
    if (!renderRefs.count(pos)) return;
    auto refIter = renderRefs.at(pos);

    if (!refIter->get()->updateChunkUse(pos, false)) {
        renderElems.erase(refIter);
        renderRefs.erase(pos);
    }
}

void LocalDimension::addLocalEntity(std::shared_ptr<LocalLuaEntity> &entity) {
    localEntities.push_back(entity);
    localEntityRefs.emplace(entity->id, --localEntities.end());
}

void LocalDimension::removeLocalEntity(std::shared_ptr<LocalLuaEntity> &entity) {
    if (!localEntityRefs.count(entity->id)) return;
    auto refIter = localEntityRefs.at(entity->id);

    localEntities.erase(refIter);
    localEntityRefs.erase(entity->id);
}

void LocalDimension::handleServerEntity(const Packet& p) {
    Deserializer d(p.data);

    auto id           = d.read<unsigned int>();
    auto position     = d.read<glm::vec3>();
    auto visualOffset = d.read<glm::vec3>();
    auto rotation     = d.read<glm::vec3>();
    auto scale        = d.read<float>();
    auto displayMode  = d.read<std::string>();
    auto displayArg1  = d.read<std::string>();
    auto displayArg2  = d.read<std::string>();

    if (serverEntityRefs.count(id)) {
        auto& luaEntity = *serverEntityRefs.at(id)->get();
        auto& entity = *luaEntity.entity;

        entity.interpPos(position);
        entity.interpVisualOffset(visualOffset);
        entity.interpRotateX(rotation.x);
        entity.interpRotateY(rotation.y);
        entity.interpRotateZ(rotation.z);
        entity.interpScale(scale);

        luaEntity.setDisplayType(displayMode, displayArg1, displayArg2);
    }
    else {
        auto entity = std::make_shared<ServerLocalLuaEntity>(id, defs, displayMode, displayArg1, displayArg2);
        entity->entity->setPos(position);
        entity->entity->setVisualOffset(visualOffset);
        entity->entity->interpRotateX(rotation.x);
        entity->entity->interpRotateY(rotation.y);
        entity->entity->interpRotateZ(rotation.z);
        entity->entity->setScale(scale);
        serverEntities.push_back(entity);
        serverEntityRefs.emplace(id, --serverEntities.end());
    }
}

int LocalDimension::getMeshChunkCount() {
    return static_cast<int>(renderElems.size());
}

bool LocalDimension::setBlock(glm::ivec3 pos, unsigned int block) {
    bool exists = Dimension::setBlock(pos, block);
    if (!exists) return false;

    auto chunkPos = Space::Chunk::world::fromBlock(pos);
    auto chunk = getChunk(chunkPos);

    chunk->dirty = true;
    attemptMeshChunk(chunk);
    return true;
}

void LocalDimension::attemptMeshChunk(const sptr<BlockChunk>& chunk, bool updateAdjacents) {
//    if (!chunk->dirty) return; //TODO
    auto dirs = Vec::cardinalVectors;
    bool allExists = true;
    for (auto dir : dirs) {
        if (!getAdjacentExists(chunk->pos + dir, updateAdjacents)) {
            allExists = false;
        }
    }

    if (allExists) {
        if (chunk->shouldRender()) {
            pendingMesh.push_back(chunk->pos);
        }
        else {
            removeMeshChunk(chunk->pos);
        }

        chunk->dirty = false; //TODO: Make dirty work
    }
}

bool LocalDimension::getAdjacentExists(glm::vec3 pos, bool updateAdjacents) {
    auto chunk = getChunk(pos);
    if (chunk == nullptr) return false;
    if (updateAdjacents) attemptMeshChunk(chunk, false);
    return true;
}