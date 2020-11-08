//
// Created by aurailus on 04/04/19.
//

#include "LocalDimension.h"

#include "world/LocalWorld.h"
#include "util/net/NetField.h"
#include "util/net/PacketView.h"
#include "lua/usertype/Target.h"
#include "lua/usertype/Player.h"
#include "world/dim/chunk/Chunk.h"
#include "client/graph/Renderer.h"
#include "world/dim/chunk/Region.h"
#include "world/dim/chunk/MapBlock.h"
#include "client/stream/MeshGenStream.h"
#include "client/graph/mesh/MeshChunk.h"

LocalDimension::LocalDimension(SubgamePtr game, LocalWorld& world, const std::string& identifier, unsigned int ind) :
    Dimension(game, static_cast<World&>(world), identifier, ind), meshGenStream(std::make_shared<MeshGenStream>(game, *this)) {}


/**
 * Deactivates a dimension, cleaning up all mesh chunks.
 * Called on the active dimension when the player leaves it.
 */

void LocalDimension::deactivate() {
    while (!renderElems.empty()) {
        auto chunk = renderElems.front();
        renderRefs.erase(chunk->getPos());
        renderElems.pop_front();
    }

    while (!regions.empty()) {
        removeRegion(regions.begin()->first);
    }
}

void LocalDimension::update(double delta) {
    finishMeshes();

    for (auto& entity : localEntities ) entity.entity.l()->update(delta);
    for (auto& entity : serverEntities) entity.entity.l()->update(delta);
    for (auto& entity : playerEntities) entity.update(delta);

    auto clientMapBlock = Space::MapBlock::world::fromBlock(static_cast<LocalWorld&>(world).getPlayer()->getPos());

    for (auto it = regions.cbegin(); it != regions.cend();) {
        bool remove = false;
        for (unsigned short m = 0; m < 64; m++) {
            auto mapBlock = it->second->get(m);
            if (!mapBlock) continue;

            if (abs(clientMapBlock.x - mapBlock->pos.x) > LocalDimension::MB_STORE_H + 1
             || abs(clientMapBlock.y - mapBlock->pos.y) > LocalDimension::MB_STORE_V + 1
             || abs(clientMapBlock.z - mapBlock->pos.z) > LocalDimension::MB_STORE_H + 1) {

                for (unsigned short c = 0; c < 64; c++) {
                    auto chunk = mapBlock->get(c);
                    if (!chunk) continue;
                    removeMeshChunk(chunk->getPos());
                }

                it->second->remove(m);
                if (it->second->count <= 0) {
                    remove = true;
                    auto l = getWriteLock();
                    it = regions.erase(it);
                    break;
                }
            }
        }
        if (!remove) it++;
    }
}

void LocalDimension::setChunk(std::shared_ptr<Chunk> chunk) {
    Dimension::setChunk(chunk);
    attemptMeshChunk(chunk);
}

bool LocalDimension::setBlock(glm::ivec3 pos, unsigned int block) {
    bool exists = Dimension::setBlock(pos, block);
    if (!exists) return false;

    auto chunkPos = Space::Chunk::world::fromBlock(pos);
    auto chunk = getChunk(chunkPos);

    chunk->setDirty(true);

    auto lp = Space::Block::relative::toChunk(pos);
    auto cp = Space::Chunk::world::fromBlock(pos);

    std::shared_ptr<Chunk> tempChunk;
    if (lp.x == 15 && (tempChunk = getChunk(cp + glm::ivec3 {1, 0, 0}))) tempChunk->setDirty(true);
    else if (lp.x == 0 && (tempChunk = getChunk(cp + glm::ivec3 {-1, 0, 0}))) tempChunk->setDirty(true);
    if (lp.y == 15 && (tempChunk = getChunk(cp + glm::ivec3 {0, 1, 0}))) tempChunk->setDirty(true);
    else if (lp.y == 0 && (tempChunk = getChunk(cp + glm::ivec3 {0, -1, 0}))) tempChunk->setDirty(true);
    if (lp.z == 15 && (tempChunk = getChunk(cp + glm::ivec3 {0, 0, 1}))) tempChunk->setDirty(true);
    else if (lp.z == 0 && (tempChunk = getChunk(cp + glm::ivec3 {0, 0, -1}))) tempChunk->setDirty(true);

    attemptMeshChunk(chunk, true);
    return true;
}

void LocalDimension::blockPlace(const Target &target, PlayerPtr player) {
    std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
        game->getParser().core["block_place"], Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));

    static_cast<LocalWorld&>(world).getNet().blockPlace(target);

    auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
    if (!stack) return;

    auto inv = player.l()->getInventory();
    if (inv->hasList(player->getWieldList()))
        inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void LocalDimension::blockInteract(const Target &target, PlayerPtr player) {
    game->getParser().safe_function(game->getParser().core["block_interact"],
        Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));

    static_cast<LocalWorld&>(world).getNet().blockInteract(target);
}

void LocalDimension::blockPlaceOrInteract(const Target &target, PlayerPtr player) {
    std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
        game->getParser().core["block_interact_or_place"], Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));

    static_cast<LocalWorld&>(world).getNet().blockPlaceOrInteract(target);

    auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
    if (!stack) return;

    auto inv = player.l()->getInventory();
    if (inv->hasList(player->getWieldList()))
        inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

double LocalDimension::blockHit(const Target &target, PlayerPtr player) {
    double timeout = 0, damage = 0;
    sol::tie(damage, timeout) = game->getParser().safe_function(game->getParser().core["block_hit"],
        Api::Usertype::LocalPlayer(player.l()), Api::Usertype::Target(target));

    static_cast<LocalWorld&>(world).getNet().blockHit(target);

    return timeout;
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

void LocalDimension::addLocalEntity(Api::Usertype::Entity entity) {
    unsigned int id = entity.get_id();
    localEntities.push_back(entity);
    localEntityRefs.emplace(id, --localEntities.end());
}

void LocalDimension::removeLocalEntity(Api::Usertype::Entity entity) {
    unsigned int id = entity.get_id();

    if (!localEntityRefs.count(id)) return;
    auto refIter = localEntityRefs.at(id);

    localEntities.erase(refIter);
    localEntityRefs.erase(id);
}

void LocalDimension::serverEntitiesInfo(Deserializer& e) {
    bool boolean;
    unsigned int x, y;
    std::string type, a, b;

    e.read<unsigned int>();

    while (!e.atEnd()) {
        std::string dat = e.read<std::string>();
        Deserializer d(dat);

        unsigned int id = d.read<unsigned int>();
        std::shared_ptr<LocalLuaEntity> activeEntity;
        if (serverEntityRefs.count(id)) activeEntity = serverEntityRefs.at(id)->entity.l();
        else {
            auto ent = std::make_shared<LocalLuaEntity>(game, world.getDimension(getInd()));
            auto entity = Api::Usertype::Entity(ent);
            ent->setId(id);
            serverEntities.push_back(entity);
            serverEntityRefs.emplace(id, --serverEntities.end());
            activeEntity = ent;
        }

        while (!d.atEnd()) {
            switch (d.readE<NetField>()) {
                case NetField::DIM: activeEntity->setDim(world.getDimension(d.read<unsigned int>())); break;
                case NetField::POS: activeEntity->setPos(d.read<glm::vec3>()); break;
                case NetField::VEL: activeEntity->setVel(d.read<glm::vec3>()); break;
                case NetField::ROT: activeEntity->setRot(d.read<glm::vec3>()); break;
                case NetField::SCALE: activeEntity->setScale(d.read<glm::vec3>()); break;
                case NetField::VISUAL_OFF: activeEntity->setVisualOffset(d.read<glm::vec3>()); break;
                case NetField::ANIM_STATE: d.read(boolean); activeEntity->animation.setPlaying(boolean); break;
                case NetField::DISPLAY: d.read(type).read(a).read(b); activeEntity->setAppearance(type, a, b); break;
                case NetField::ANIM_RANGE: d.read(x).read(y).read(boolean); activeEntity->animation.setAnim(glm::ivec2 {x, y}, 10, boolean); break;
            }
        }
    }
}

void LocalDimension::serverEntitiesRemoved(Deserializer& d) {
    d.read<unsigned int>();
    while (!d.atEnd()) {
        unsigned int id = d.read<unsigned int>();
        if (!serverEntityRefs.count(id)) continue;
        auto refIter = serverEntityRefs.at(id);
        serverEntities.erase(refIter);
        serverEntityRefs.erase(id);
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
    for (auto& entity : localEntities) entity.entity.l()->draw(renderer);
    for (auto& entity : serverEntities) entity.entity.l()->draw(renderer);
    for (auto& entity : playerEntities) entity.draw(renderer);
}

int LocalDimension::getMeshChunkCount() {
    return static_cast<int>(renderElems.size());
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

void LocalDimension::attemptMeshChunk(const std::shared_ptr<Chunk>& chunk, bool priority, bool updateAdjacents) {
    bool renderable = true;
    for (auto dir : Vec::TO_VEC) if (!getAdjacentExists(chunk->getPos() + dir, updateAdjacents)) renderable = false;
    if (!renderable) return;

    if (!chunk->isDirty()) return;
    if (!chunk->chunkShouldRender()) removeMeshChunk(chunk->getPos());

    meshGenStream->queue(chunk->getPos(), priority);
    chunk->setDirty(false);
}

bool LocalDimension::getAdjacentExists(glm::vec3 pos, bool updateAdjacents) {
    auto chunk = getChunk(pos);
    if (chunk == nullptr) return false;
    if (updateAdjacents) attemptMeshChunk(chunk, false, false);
    return true;
}