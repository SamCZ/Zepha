//
// Created by aurailus on 01/10/19.
//

#pragma once

#include <list>

#include "Dimension.h"

#include "../lua/usertype/Entity.h"

class ServerWorld;
class ServerPlayer;
class ServerSubgame;

class ServerDimension : public Dimension {
public:
    ServerDimension(SubgamePtr game, ServerWorld& world, const std::string& identifier, unsigned int ind);

    virtual void update(double delta) override;

    void setChunk(std::shared_ptr<Chunk> chunk) override;
    bool setBlock(glm::ivec3 pos, unsigned int block) override;

    virtual void blockPlace(const Target &target, PlayerPtr player) override;
    virtual void blockPlaceOrInteract(const Target &target, PlayerPtr player) override;
    virtual void blockInteract(const Target &target, PlayerPtr player) override;
    virtual double blockHit(const Target &target, PlayerPtr player) override;

    void addLuaEntity(Api::Usertype::Entity entity);
    void removeLuaEntity(Api::Usertype::Entity entity);

//    unsigned long long getMapBlockIntegrity(glm::ivec3 mapBlock);

    std::list<Api::Usertype::Entity>& getLuaEntities();
    const std::list<unsigned int>& getRemovedEntities() const;
    void clearRemovedEntities();

private:
    typedef std::list<Api::Usertype::Entity>::iterator luaent_ref;

    std::unordered_map<unsigned int, luaent_ref> luaEntityRefs {};
    std::list<Api::Usertype::Entity> luaEntities {};
    std::list<unsigned int> removedEntities {};

//    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};

