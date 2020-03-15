//
// Created by aurailus on 04/04/19.
//

#include "LocalDimension.h"

#include "../game/scene/world/graph/MeshChunk.h"

LocalDimension::LocalDimension(ClientGame &defs) : defs(defs), meshGenStream(std::make_unique<MeshGenStream>(defs, *this)) {}

void LocalDimension::update(double delta, glm::vec3 playerPos) {
    finishMeshes();
    queueMeshes();

    for (auto& entity : localEntities ) entity->entity->update(delta);
    for (auto& entity : serverEntities) entity->entity->update(delta);
    for (auto& entity : playerEntities) entity.update(delta);

    auto clientMapBlock = Space::MapBlock::world::fromBlock(playerPos);


    for (auto it = regions.cbegin(); it != regions.cend();) {
        bool remove = false;
        for (unsigned short m = 0; m < 64; m++) {
            auto mapBlock = it->second->operator[](m);
            if (!mapBlock) continue;

            if (abs(clientMapBlock.x - mapBlock->pos.x) > LocalDimension::MB_STORE_H + 1
             || abs(clientMapBlock.y - mapBlock->pos.y) > LocalDimension::MB_STORE_V + 1
             || abs(clientMapBlock.z - mapBlock->pos.z) > LocalDimension::MB_STORE_H + 1) {

                for (unsigned short c = 0; c < 64; c++) {
                    auto chunk = mapBlock->operator[](c);
                    if (!chunk) continue;

                    removeMeshChunk(chunk->pos);
                }

                it->second->remove(m);
                if (it->second->count <= 0) {
                    remove = true;
                    it = regions.erase(it);
                    break;
                }
            }
        }
        if (!remove) it++;
    }

//    for (const auto& region : regions) {
//        for (unsigned short i = 0; i < 64; i++) {
//            auto mapBlock = region.second->operator[](i);
//            if (mapBlock == nullptr) continue;
//
//            if (abs(clientMapBlock.x - mapBlock->pos.x) > LocalDimension::MB_STORE_H + 1
//             || abs(clientMapBlock.y - mapBlock->pos.y) > LocalDimension::MB_STORE_V + 1
//             || abs(clientMapBlock.z - mapBlock->pos.z) > LocalDimension::MB_STORE_H + 1) {
//
//                for (unsigned short j = 0; j < 64; j++) {
//                    auto chunk = mapBlock->operator[](j);
//                    if (chunk == nullptr) continue;
//                    removeMeshChunk(chunk->pos);
//                }
//
//                removeMapBlock(mapBlock->pos);
//                if (region.second->count == 0) break;
//            }
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
        }
        else removeMeshChunk(meshDetails->pos);

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

void LocalDimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
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

void LocalDimension::serverEntityInfo(PacketView& p) {
    auto id           = p.d.read<unsigned int>();
    auto position     = p.d.read<glm::vec3>();
    auto visualOffset = p.d.read<glm::vec3>();
    auto rotation     = p.d.read<glm::vec3>();
    auto scale        = p.d.read<float>();
    auto displayMode  = p.d.read<std::string>();
    auto displayArg1  = p.d.read<std::string>();
    auto displayArg2  = p.d.read<std::string>();

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

void LocalDimension::serverEntityRemoved(unsigned int id) {
    if (!serverEntityRefs.count(id)) return;
    auto refIter = serverEntityRefs.at(id);

    serverEntities.erase(refIter);
    serverEntityRefs.erase(id);
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

void LocalDimension::attemptMeshChunk(const std::shared_ptr<BlockChunk>& chunk, bool updateAdjacents) {
//    if (!chunk->dirty) return; //TODO
    auto dirs = Vec::cardinalVectors;
    bool allExists = true;
    for (auto dir : dirs) {
        if (!getAdjacentExists(chunk->pos + dir, updateAdjacents)) {
            allExists = false;
        }
    }

    if (allExists) {
        if (chunk->shouldHaveMesh) {
            pendingMesh.push_back(chunk->pos);
        }
        else removeMeshChunk(chunk->pos);

        chunk->dirty = false; //TODO: Make dirty work
    }
}

bool LocalDimension::getAdjacentExists(glm::vec3 pos, bool updateAdjacents) {
    auto chunk = getChunk(pos);
    if (chunk == nullptr) return false;
    if (updateAdjacents) attemptMeshChunk(chunk, false);
    return true;
}