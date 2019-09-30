//
// Created by aurailus on 14/12/18.
//

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

#include "LocalWorld.h"
#include "../net/ClientNetworkInterpreter.h"

LocalWorld::LocalWorld(LocalDefs& defs, glm::vec3* playerPos, ClientNetworkInterpreter* server) :
    playerPos(playerPos),
    dimension(defs),
    server(server),
    defs(defs) {}

void LocalWorld::init() {
    delete worldGenStream;
    worldGenStream = new WorldInterpolationStream(55, defs);
}

void LocalWorld::update(double delta) {
    playerChunkPos = TransPos::roundPos(*playerPos / glm::vec3(TransPos::CHUNK_SIZE));

    finishChunks();

    updateBlockDamages(delta);
    dimension.update(*playerPos);

    auto end = particles.begin();
    for (auto i = particles.begin(); i < particles.end(); i++) {
        (*i)->update(delta, *playerPos);
        if ((*i)->time > 1) {
            end = i;
            delete (*i);
        }
    }
    if (end != particles.begin()) particles.erase(particles.begin(), end + 1);

    for (auto it = luaEntities.begin(); it != luaEntities.end();) {
        if (auto ptr = (*it).lock()) {
            ptr->update(delta);
            it++;
        }
        else it = luaEntities.erase(it);
    }
}

void LocalWorld::addEntity(const wptr<Entity> &entity) {
    luaEntities.push_back(entity);
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

    auto def = defs.defs().blockFromId(getBlock(pos));
    for (int i = 0; i < 40 * amount; i++) {
        auto p = new ParticleEntity(pos, def);
        particles.push_back(p);
    }
}

void LocalWorld::finishChunks() {
    auto finishedChunks = worldGenStream->update();

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

void LocalWorld::loadChunkPacket(Packet p) {
    worldGenStream->pushBack(p);
}

std::shared_ptr<BlockChunk> LocalWorld::getChunk(glm::vec3 chunkPos) {
    return dimension.getChunk(chunkPos);
}

void LocalWorld::commitChunk(glm::vec3 pos, std::shared_ptr<BlockChunk> c) {
    dimension.setChunk(std::move(c));
}

int LocalWorld::renderChunks(Renderer &renderer) {
    return dimension.render(renderer);
}

void LocalWorld::renderEntities(Renderer &renderer) {
    for (auto block : crackedBlocks) block->draw(renderer);
    for (auto &p : particles) p->draw(renderer);

    for (auto it = luaEntities.begin(); it != luaEntities.end();) {
        if (auto ptr = (*it).lock()) {
            ptr->draw(renderer);
            it++;
        } else it = luaEntities.erase(it);
    }
}

int LocalWorld::getMeshChunkCount() {
    return dimension.getMeshChunkCount();
}

unsigned int LocalWorld::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBlock(local);
    return DefinitionAtlas::INVALID;
}

void LocalWorld::localSetBlock(glm::vec3 pos, unsigned int block) {
    if (block == LocalDefinitionAtlas::AIR) {
        auto def = defs.defs().blockFromId(getBlock(pos));
        if (def.callbacks.count(Callback::BREAK_CLIENT)) {
            def.callbacks[Callback::BREAK_CLIENT](defs.lua().vecToTable(pos));
        }
    }
    else {
        auto def = defs.defs().blockFromId(block);
        if (def.callbacks.count(Callback::PLACE_CLIENT)) {
            def.callbacks[Callback::PLACE_CLIENT](defs.lua().vecToTable(pos));
        }
    }

    server->setBlock(pos, block);
    dimension.setBlock(pos, block);
}

void LocalWorld::setBlock(glm::vec3 pos, unsigned int block) {
    dimension.setBlock(pos, block);
}

bool LocalWorld::solidAt(glm::vec3 pos) {
    return defs.defs().blockFromId(getBlock(pos)).solid;
}
