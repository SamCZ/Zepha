//
// Created by aurailus on 01/10/19.
//

#include "../def/gen/MapGen.h"
#include "../server/conn/ServerClient.h"
#include "../server/world/ServerWorld.h"
#include "ServerDimension.h"

void ServerDimension::update(const std::vector<ServerClient*> &clients) {
    for (const auto& region : regions) {
        for (unsigned short i = 0; i < 64; i++) {
            if (region.second->operator[](i) == nullptr) continue;
            const auto& mapBlockPos = region.second->operator[](i)->pos;

            bool clientNearby = false;
            for (const auto& client : clients) {
                if (client->hasPlayer()) {
                    auto clientPos = Space::MapBlock::world::fromChunk(client->cGetPlayer().getChunkPos());
                    if (abs(clientPos.x - mapBlockPos.x) <= ServerWorld::MB_GEN_H + 1
                     && abs(clientPos.y - mapBlockPos.y) <= ServerWorld::MB_GEN_V + 1
                     && abs(clientPos.z - mapBlockPos.z) <= ServerWorld::MB_GEN_H + 1) {
                        clientNearby = true;
                        break;
                    }
                }
            }

            if (!clientNearby) region.second->remove(i);
        }
    }
}

bool ServerDimension::setBlock(glm::ivec3 pos, unsigned int block) {
    bool manip = Dimension::setBlock(pos, block);
    if (!manip) return false;
    glm::vec3 mb = Space::MapBlock::world::fromBlock(pos);
    mapBlockIntegrity[mb] = mapBlockIntegrity[mb] + 1;
    return true;
}

void ServerDimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
    // Combine partials if there are any
    std::shared_ptr<BlockChunk> existing = getChunk(chunk->pos);
    if (existing != nullptr) chunk = MapGen::combinePartials(chunk, existing);

    Dimension::setChunk(chunk);
    glm::vec3 mb = Space::MapBlock::world::fromChunk(chunk->pos);
    mapBlockIntegrity[mb] = mapBlockIntegrity[mb] + 1;
}

void ServerDimension::addLuaEntity(std::shared_ptr<ServerLuaEntity> &entity) {
    luaEntities.push_back(entity);
    luaEntityRefs.emplace(entity->id, --luaEntities.end());
}

void ServerDimension::removeLuaEntity(std::shared_ptr<ServerLuaEntity> &entity) {
    if (!luaEntityRefs.count(entity->id)) return;
    auto refIter = luaEntityRefs.at(entity->id);

    luaEntities.erase(refIter);
    luaEntityRefs.erase(entity->id);
}

unsigned long long ServerDimension::getMapBlockIntegrity(glm::ivec3 mapBlock) {
    if (mapBlockIntegrity.count(mapBlock)) return mapBlockIntegrity[mapBlock];
    return 0;
}

std::list<std::shared_ptr<ServerLuaEntity>> &ServerDimension::getLuaEntities() {
    return luaEntities;
}