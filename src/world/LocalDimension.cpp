//
// Created by aurailus on 04/04/19.
//

#include "LocalDimension.h"

#include "../net/PacketView.h"
#include "../world/chunk/Chunk.h"
#include "../world/chunk/Region.h"
#include "../game/graph/Renderer.h"
#include "../world/chunk/MapBlock.h"
#include "../def/LocalDefinitionAtlas.h"
#include "../lua/api/class/LocalLuaEntity.h"
#include "../game/scene/world/MeshGenStream.h"
#include "../game/scene/world/graph/MeshChunk.h"
#include "../game/scene/world/ChunkMeshDetails.h"
#include "../lua/api/class/ServerLocalLuaEntity.h"

LocalDimension::LocalDimension(LocalSubgame &game) : Dimension(*game.defs),
    meshGenStream(std::make_shared<MeshGenStream>(game, *this)),
    game(game) {}

void LocalDimension::update(double delta, glm::vec3 playerPos) {
    finishMeshes();

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
}

void LocalDimension::finishMeshes() {
    lastMeshUpdates = 0;
    auto finishedMeshes = meshGenStream->update();

    for (ChunkMeshDetails* meshDetails : finishedMeshes) {

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

void LocalDimension::setChunk(std::shared_ptr<Chunk> chunk) {
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
        auto entity = std::make_shared<ServerLocalLuaEntity>(id, game, displayMode, displayArg1, displayArg2);
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

    auto lp = Space::Block::relative::toChunk(pos);
    auto cp = Space::Chunk::world::fromBlock(pos);

    if (lp.x == 15 && getChunk(cp + glm::ivec3 {1, 0, 0})) getChunk(cp + glm::ivec3 {1, 0, 0})->dirty = true;
    else if (lp.x == 0 && getChunk(cp + glm::ivec3 {-1, 0, 0})) getChunk(cp + glm::ivec3 {-1, 0, 0})->dirty = true;
    if (lp.y == 15 && getChunk(cp + glm::ivec3 {0, 1, 0})) getChunk(cp + glm::ivec3 {0, 1, 0})->dirty = true;
    else if (lp.y == 0 && getChunk(cp + glm::ivec3 {0, -1, 0})) getChunk(cp + glm::ivec3 {0, -1, 0})->dirty = true;
    if (lp.z == 15 && getChunk(cp + glm::ivec3 {0, 0, 1})) getChunk(cp + glm::ivec3 {0, 0, 1})->dirty = true;
    else if (lp.z == 0 && getChunk(cp + glm::ivec3 {0, 0, -1})) getChunk(cp + glm::ivec3 {0, 0, -1})->dirty = true;

    attemptMeshChunk(chunk, true);
    return true;
}

void LocalDimension::attemptMeshChunk(const std::shared_ptr<Chunk>& chunk, bool priority, bool updateAdjacents) {
    static const std::vector<glm::ivec3> dirs {
            glm::ivec3 {1, 0, 0}, glm::ivec3 {-1, 0, 0},
            glm::ivec3 {0, 1, 0}, glm::ivec3 {0, -1, 0},
            glm::ivec3 {0, 0, 1}, glm::ivec3 {0, 0, -1}};

    bool renderable = true;
    for (auto dir : dirs) if (!getAdjacentExists(chunk->pos + dir, updateAdjacents)) renderable = false;
    if (!renderable) return;

    if (!chunk->dirty) return;
    if (!chunk->shouldRender) removeMeshChunk(chunk->pos);


    meshGenStream->queue(chunk->pos, priority);
    chunk->dirty = false;
}

bool LocalDimension::getAdjacentExists(glm::vec3 pos, bool updateAdjacents) {
    auto chunk = getChunk(pos);
    if (chunk == nullptr) return false;
    if (updateAdjacents) attemptMeshChunk(chunk, false, false);
    return true;
}

std::unordered_set<glm::ivec3, Vec::ivec3> LocalDimension::propogateAddNodes() {
    auto updated = Dimension::propogateAddNodes();
    for (auto& update : updated) attemptMeshChunk(getChunk(update));
    return {};
}

std::unordered_set<glm::ivec3, Vec::ivec3> LocalDimension::propogateRemoveNodes() {
    auto updated = Dimension::propogateRemoveNodes();
    for (auto& update : updated) attemptMeshChunk(getChunk(update));
    return {};
}
