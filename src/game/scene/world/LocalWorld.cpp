//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "Player.h"
#include "../../../net/PacketView.h"
#include "WorldInterpolationStream.h"
#include "../../../world/chunk/Chunk.h"
#include "../../../def/item/BlockDef.h"
#include "../../../def/gen/LocalBiomeAtlas.h"
#include "../../../def/LocalDefinitionAtlas.h"
#include "../../../lua/LocalLuaParser.h"
#include "../../entity/engine/ParticleEntity.h"
#include "../../entity/engine/BlockCrackEntity.h"
#include "../../../lua/usertype/LocalLuaPlayer.h"
#include "../../../net/client/ClientNetworkInterpreter.h"

LocalWorld::LocalWorld(LocalSubgame& defs, ClientNetworkInterpreter* server) :
        defs(defs),
        net(server),
        dimension(defs) {}

void LocalWorld::init(Player* player) {
    this->player = player;
    delete worldGenStream;
    worldGenStream = new WorldInterpolationStream(55, defs);
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

void LocalWorld::blockPlace(glm::vec3 pos, unsigned int block) {
    if (block == LocalDefinitionAtlas::AIR) {
        auto& def = defs.defs->blockFromId(getBlock(pos));
        if (def.callbacks.count(Callback::BREAK_CLIENT))
            defs.lua->safe_function(def.callbacks[Callback::BREAK_CLIENT], pos);
    }
    else {
        auto& def = defs.defs->blockFromId(block);
        if (def.callbacks.count(Callback::PLACE_CLIENT))
            defs.lua->safe_function(def.callbacks[Callback::PLACE_CLIENT], pos);
    }

    net->blockPlace(pos, block);
    dimension.setBlock(pos, block);
}

void LocalWorld::blockBreak(glm::vec3 pos) {
    blockPlace(pos, DefinitionAtlas::AIR);
}

void LocalWorld::blockInteract(PointedThing &thing) {
    auto& def = defs.defs->blockFromId(getBlock(thing.target.block.pos));

    if (def.callbacks.count(Callback::INTERACT_CLIENT))
        defs.lua->safe_function(def.callbacks[Callback::INTERACT_CLIENT], thing.target.block.pos);

    net->blockInteract(thing.target.block.pos);
}

double LocalWorld::blockHit(PointedThing& thing) {
    glm::ivec3 pos = thing.target.block.pos;

    auto& blockDef = defs.defs->blockFromId(getBlock(thing.target.block.pos));

    double damage = 0, timeout = 0;
    sol::tie(damage, timeout) = defs.lua->safe_function(defs.lua->core["get_hit_impact"],
        defs.lua->core.get<LocalLuaPlayer>("player"), blockDef.identifier);

    if (damage == 0) return timeout;

    BlockCrackEntity* block = nullptr;
    for (auto test : crackedBlocks) if (glm::ivec3(test->getPos()) == pos) { block = test; break; }
    if (block == nullptr) {
        block = new BlockCrackEntity(blockDef, defs.textures, pos);
        crackedBlocks.push_back(block);
    }

    block->addDamage(damage);
    block->time = 0;

//    auto def = defs.defs.blockFromId(getBlock(pos));
//    for (int i = 0; i < 40 * damage; i++) {
//        auto p = new ParticleEntity(pos, def);
//        particles.push_back(p);
//    }

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
    for (auto block : crackedBlocks) block->draw(renderer);
    for (auto &p : particles) p->draw(renderer);

    dimension.renderEntities(renderer);
}

void LocalWorld::updateBlockDamages(double delta) {
    auto it = crackedBlocks.cbegin();
    while (it != crackedBlocks.cend()) {
        bool deleteMe = false;

        auto curr = it++;
        auto block = *curr;

        block->time += delta;

        if (block->damage >= block->maxHealth) {
            blockBreak(block->getPos());
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
            delete *curr;
            it = crackedBlocks.erase(curr);
        }
        else block->update();
    }
}

void LocalWorld::finishChunks() {
    auto finishedChunks = worldGenStream->update();

    mapBlocksInterpolated = finishedChunks->size() / 64;
    for (const auto &chunk : *finishedChunks) commitChunk(chunk);
}