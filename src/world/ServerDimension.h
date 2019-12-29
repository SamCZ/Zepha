//
// Created by aurailus on 01/10/19.
//

#pragma once

#include "Dimension.h"
#include "../lua/api/type/ServerLuaEntity.h"

class ServerDimension : public Dimension {
public:
    ServerDimension() = default;

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::vec3 pos, unsigned int block) override;

    void addLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);
    void removeLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);

    unsigned long long getMapBlockIntegrity(glm::vec3 mapBlock);

private:
    typedef std::list<sptr<ServerLuaEntity>>::iterator luaent_ref;

    std::unordered_map<uint, luaent_ref> luaEntityRefs {};
    std::list<std::shared_ptr<ServerLuaEntity>> luaEntities {};

    std::unordered_map<glm::vec3, unsigned long long, Vec::compareFunc> mapBlockIntegrity {};
};

