//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "Player.h"
#include "../../../net/PacketView.h"
#include "WorldInterpolationStream.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../def/item/BlockDef.h"
#include "../../../lua/usertype/Target.h"
#include "../../../lua/usertype/LuaItemStack.h"
#include "../../inventory/LocalInventoryList.h"
#include "../../entity/engine/ParticleEntity.h"
#include "../../entity/engine/BlockCrackEntity.h"
#include "../../../lua/usertype/LocalLuaPlayer.h"
#include "../../../net/client/ClientNetworkInterpreter.h"

LocalWorld::LocalWorld(LocalSubgame& defs, ClientNetworkInterpreter* server) :
    World(defs),
    game(defs),
    net(server),
    dimension(defs) {}

void LocalWorld::init(Player* player) {
    this->player = player;
    delete worldGenStream;
    worldGenStream = new WorldInterpolationStream(55, game);
}

void LocalWorld::update(double delta) {
    finishChunks();
    updateBlockDamages(delta);
    dimension.update(delta, player->getPos());
    lastMeshUpdates = dimension.lastMeshUpdates;

    auto end = particles.begin();
    for (auto i = particles.begin(); i < particles.end(); i++) {
        (*i)->update(delta, player->getPos());
        if ((*i)->time > 1) {
            end = i;
            delete (*i);
        }
    }
    if (end != particles.begin()) particles.erase(particles.begin(), end + 1);
}

void LocalWorld::loadWorldPacket(std::unique_ptr<PacketView> p) {
    worldGenStream->queuePacket(std::move(p));
}

void LocalWorld::commitChunk(std::shared_ptr<Chunk> c) {
    dimension.setChunk(std::move(c));
}

unsigned int LocalWorld::getBlock(glm::ivec3 pos) {
    return dimension.getBlock(pos);
}

void LocalWorld::setBlock(glm::ivec3 pos, unsigned int block) {
    dimension.setBlock(pos, block);
}

double LocalWorld::setBlockDamage(glm::ivec3 pos, double damage) {
    double totalDamage = World::setBlockDamage(pos, damage);

    BlockCrackEntity* block = nullptr;
    if (crackEntities.count(pos)) block = crackEntities[pos];
    else block = new BlockCrackEntity(game.defs->blockFromId(getBlock(pos)), game.textures, pos);
    block->setDamage(damage);
    block->time = 0;

    return totalDamage;
}

void LocalWorld::blockPlace(Target& target) {
    std::tuple<sol::optional<LuaItemStack>, sol::optional<glm::vec3>> res = game.lua->safe_function(
        game.lua->core["block_place"], LocalLuaPlayer(*player), Api::Usertype::Target(target));

    auto stack = std::get<sol::optional<LuaItemStack>>(res);
    if (stack) player->getWieldList()->setStack(player->getWieldIndex(), ItemStack(*stack, game.getDefs()));
    net->blockPlace(target);
}

void LocalWorld::blockInteract(Target &target) {
    game.lua->safe_function(game.lua->core["block_interact"],
        LocalLuaPlayer(*player), Api::Usertype::Target(target));

    net->blockInteract(target);
}

void LocalWorld::blockPlaceOrInteract(Target &target) {
    std::tuple<sol::optional<LuaItemStack>, sol::optional<glm::vec3>> res = game.lua->safe_function(
        game.lua->core["block_interact_or_place"], LocalLuaPlayer(*player), Api::Usertype::Target(target));

    auto stack = std::get<sol::optional<LuaItemStack>>(res);
    if (stack) player->getWieldList()->setStack(player->getWieldIndex(), ItemStack(*stack, game.getDefs()));
    net->blockPlaceOrInteract(target);
}

double LocalWorld::blockHit(Target& target) {
    double timeout = 0, damage = 0;
    sol::tie(damage, timeout) = game.lua->safe_function(game.lua->core["block_hit"],
        LocalLuaPlayer(*player), Api::Usertype::Target(target));

//    net->blockHit(target);

    return timeout;
}

unsigned short LocalWorld::getBiome(glm::vec3 pos) {
    auto chunkPos = Space::Chunk::world::fromBlock(pos);
    auto local = Space::Block::relative::toChunk(pos);

    auto chunk = getChunk(chunkPos);
    auto l = chunk->aquireLock();
    if (chunk != nullptr) return chunk->getBiome(local);
    return BiomeAtlas::INVALID;
}

std::shared_ptr<Chunk> LocalWorld::getChunk(glm::ivec3 pos) {
    return dimension.getChunk(pos);
}

int LocalWorld::getMeshChunkCount() {
    return dimension.getMeshChunkCount();
}

int LocalWorld::renderChunks(Renderer &renderer) {
    return dimension.renderChunks(renderer);
}

void LocalWorld::renderEntities(Renderer &renderer) {
    for (auto& block : crackEntities) block.second->draw(renderer);
    for (auto& particle : particles) particle->draw(renderer);

    dimension.renderEntities(renderer);
}

void LocalWorld::updateBlockDamages(double delta) {
    auto it = crackEntities.cbegin();
    while (it != crackEntities.cend()) {
        bool deleteMe = false;

        auto curr = it++;
        auto block = curr->second;

        block->time += delta;

        if (block->damage >= block->maxHealth) {
            //Todo: Lua callback~
            setBlock(block->getPos(), DefinitionAtlas::AIR);
            setBlockDamage(block->getPos(), 0);
            deleteMe = true;
        }

        if (block->time > 2) {
            block->update();
            block->addDamage(-1);
            block->time = 0;
        }

        if (block->damage < 0 || block->def.index != getBlock(block->getPos())) {
            deleteMe = true;
        }

        if (deleteMe) {
            delete block;
            it = crackEntities.erase(curr);
        }
        else block->update();
    }
}

void LocalWorld::finishChunks() {
    auto finishedChunks = worldGenStream->update();

    mapBlocksInterpolated = finishedChunks->size() / 64;
    for (const auto &chunk : *finishedChunks) commitChunk(chunk);
}