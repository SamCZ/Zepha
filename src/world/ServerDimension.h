//
// Created by aurailus on 01/10/19.
//

#pragma once

#include "Dimension.h"
#include "../def/gen/MapGen.h"
#include "../lua/api/type/ServerLuaEntity.h"

class ServerDimension : public Dimension {
public:
    ServerDimension() = default;

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    void addLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);
    void removeLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);

    unsigned long long getMapBlockIntegrity(glm::ivec3 mapBlock);
    std::list<std::shared_ptr<ServerLuaEntity>>& getLuaEntities();
private:
    typedef std::list<std::shared_ptr<ServerLuaEntity>>::iterator luaent_ref;

    std::unordered_map<unsigned int, luaent_ref> luaEntityRefs {};
    std::list<std::shared_ptr<ServerLuaEntity>> luaEntities {};

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};

