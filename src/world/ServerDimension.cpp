//
// Created by aurailus on 01/10/19.
//

#include "ServerDimension.h"

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
