//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "Player.h"
#include "WorldInterpolationStream.h"
#include "../net/ClientNetworkInterpreter.h"
#include "../../entity/engine/BlockCrackEntity.h"
#include "../../entity/engine/ParticleEntity.h"

LocalWorld::LocalWorld(ClientGame& defs, ClientNetworkInterpreter* server) :
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

void LocalWorld::loadChunkPacket(std::unique_ptr<Packet> p) {
    worldGenStream->pushBack(std::move(p));
}

void LocalWorld::commitChunk(std::shared_ptr<BlockChunk> c) {
    dimension.setChunk(std::move(c));
}

unsigned int LocalWorld::getBlock(glm::ivec3 pos) {
    return dimension.getBlock(pos);
}

void LocalWorld::setBlock(glm::ivec3 pos, unsigned int block) {
    dimension.setBlock(pos, block);
}

void LocalWorld::localSetBlock(glm::ivec3 pos, unsigned int block) {
    if (block == LocalDefinitionAtlas::AIR) {
        auto def = defs.defs.blockFromId(getBlock(pos));
        if (def.callbacks.count(Callback::BREAK_CLIENT)) {
            def.callbacks[Callback::BREAK_CLIENT](defs.parser.luaVec(pos));
        }
    }
    else {
        auto def = defs.defs.blockFromId(block);
        if (def.callbacks.count(Callback::PLACE_CLIENT)) {
            def.callbacks[Callback::PLACE_CLIENT](defs.parser.luaVec(pos));
        }
    }

    net->setBlock(pos, block);
    dimension.setBlock(pos, block);
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
        auto blockID = getBlock(pos);
        block = new BlockCrackEntity(defs, pos, blockID);
        crackedBlocks.push_back(block);
    }

    block->setNewDamage(block->targetDamage + amount);
    block->time = 0;

    auto def = defs.defs.blockFromId(getBlock(pos));
    for (int i = 0; i < 40 * amount; i++) {
        auto p = new ParticleEntity(pos, def);
        particles.push_back(p);
    }
}

unsigned short LocalWorld::getBiome(glm::vec3 pos) {
    auto chunkPos = Space::Chunk::world::fromBlock(pos);
    auto local = Space::Block::relative::toChunk(pos);

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBiome(local);
    return BiomeAtlas::INVALID;
}

std::shared_ptr<BlockChunk> LocalWorld::getChunk(glm::ivec3 pos) {
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

        if (block->damage >= 1) {
            localSetBlock(block->blockPos, DefinitionAtlas::AIR);
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

void LocalWorld::finishChunks() {
    auto finishedChunks = worldGenStream->update();

    lastGenUpdates = 0;
    for (const auto &chunk : finishedChunks) {
        commitChunk(chunk);
        lastGenUpdates++;
    }
}