//
// Created by aurailus on 01/10/19.
//

#pragma once

#include "Dimension.h"

#include <list>

class ServerWorld;
class ServerPlayer;
class ServerSubgame;
class ServerLuaEntity;

class ServerDimension : public Dimension {
public:
    ServerDimension(ServerSubgame& game, ServerWorld& world, const std::string& identifier, unsigned int ind);

    virtual void update(double delta) override;

    void setChunk(std::shared_ptr<Chunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    virtual void blockPlace(const Target &target, std::shared_ptr<Player> player) override;
    virtual void blockPlaceOrInteract(const Target &target, std::shared_ptr<Player> player) override;
    virtual void blockInteract(const Target &target, std::shared_ptr<Player> player) override;
    virtual double blockHit(const Target &target, std::shared_ptr<Player> player) override;

    void addLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);
    void removeLuaEntity(std::shared_ptr<ServerLuaEntity>& entity);

    unsigned long long getMapBlockIntegrity(glm::ivec3 mapBlock);
    std::list<std::shared_ptr<ServerLuaEntity>>& getLuaEntities();
    const std::list<unsigned int>& getRemovedEntities() const;
    void clearRemovedEntities();

private:
    typedef std::list<std::shared_ptr<ServerLuaEntity>>::iterator luaent_ref;

    std::unordered_map<unsigned int, luaent_ref> luaEntityRefs {};
    std::list<std::shared_ptr<ServerLuaEntity>> luaEntities {};
    std::list<unsigned int> removedEntities {};

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};

