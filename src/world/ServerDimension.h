//
// Created by aurailus on 01/10/19.
//

#pragma once

#include "Dimension.h"

#include <list>

class ServerSubgame;
class ServerClient;
class ServerLuaEntity;

class ServerDimension : public Dimension {
public:
    ServerDimension(ServerSubgame& game);

    void update(const std::vector<std::shared_ptr<ServerClient>>& clients, glm::ivec2 discardRange);

    void setChunk(std::shared_ptr<Chunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    void addLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);
    void removeLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);

    unsigned long long getMapBlockIntegrity(glm::ivec3 mapBlock);
    std::list<std::shared_ptr<ServerLuaEntity>>& getLuaEntities();
    const std::list<unsigned int>& getRemovedEntities() const;
    void clearRemovedEntities();
private:
    ServerSubgame& game;

    typedef std::list<std::shared_ptr<ServerLuaEntity>>::iterator luaent_ref;

    std::unordered_map<unsigned int, luaent_ref> luaEntityRefs {};
    std::list<std::shared_ptr<ServerLuaEntity>> luaEntities {};
    std::list<unsigned int> removedEntities {};

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};

