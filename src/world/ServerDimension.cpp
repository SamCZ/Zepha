//
// Created by aurailus on 01/10/19.
//

#include "ServerDimension.h"

#include "chunk/Chunk.h"
#include "chunk/Region.h"
#include "chunk/MapBlock.h"
#include "../def/gen/MapGen.h"
#include "../def/ServerSubgame.h"
#include "../lua/usertype/Player.h"
#include "../lua/usertype/Target.h"
#include "../lua/usertype/ItemStack.h"
#include "../net/server/conn/ServerPlayer.h"
#include "../net/server/world/ServerWorld.h"

ServerDimension::ServerDimension(SubgamePtr game, ServerWorld& world, const std::string& identifier, unsigned int ind) :
    Dimension(game, static_cast<World&>(world), identifier, ind) {}

void ServerDimension::update(double delta) {
    //TODO: Thiss
//    for (const auto& region : regions) {
//        for (unsigned short i = 0; i < 64; i++) {
//            if (region.second->operator[](i) == nullptr) continue;
//            const auto& mapBlockPos = region.second->operator[](i)->pos;
//
//            bool clientNearby = false;
//            for (auto& player : players) {
//                if (player->getDimension().getInd() == ind) {
//                    auto clientPos = Space::MapBlock::world::fromBlock(player->getPos());
//                    if (abs(clientPos.x - mapBlockPos.x) <= discardRange.x + 1
//                     && abs(clientPos.y - mapBlockPos.y) <= discardRange.y + 1
//                     && abs(clientPos.z - mapBlockPos.z) <= discardRange.x + 1) {
//                        clientNearby = true;
//                        break;
//                    }
//                }
//            }
//
//            if (!clientNearby) region.second->remove(i);
//        }
//    }
}

double ServerDimension::blockHit(const Target &target, PlayerPtr player) {
    double timeout = 0, damage = 0;
    sol::tie(damage, timeout) = game->getParser().safe_function(game->getParser().core["block_hit"],
        Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));

    return timeout;
}

void ServerDimension::blockPlace(const Target &target, PlayerPtr player) {
    std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
        game->getParser().core["block_place"], Api::Usertype::ServerPlayer(player.s()), Api::Usertype::Target(target));

    auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
    if (!stack) return;

    auto inv = player->getInventory();
    if (inv->hasList(player->getWieldList())) inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void ServerDimension::blockInteract(const Target &target, PlayerPtr player) {
    game->getParser().safe_function(game->getParser().core["block_interact"],
        Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));
}

void ServerDimension::blockPlaceOrInteract(const Target &target, PlayerPtr player) {
    std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
        game->getParser().core["block_interact_or_place"], Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));

    auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
    if (!stack) return;

    auto inv = player.s()->getInventory();
    if (inv->hasList(player->getWieldList())) inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void ServerDimension::setChunk(std::shared_ptr<Chunk> chunk) {
    std::shared_ptr<Chunk> existing = getChunk(chunk->getPos());
    if (existing != nullptr) chunk = combinePartials(chunk, existing);
    Dimension::setChunk(chunk);
}

void ServerDimension::addLuaEntity(Api::Usertype::Entity entity) {
    unsigned int id = entity.get_id();
    luaEntities.push_back(std::move(entity));
    luaEntityRefs.emplace(id, --luaEntities.end());
}

void ServerDimension::removeLuaEntity(Api::Usertype::Entity entity) {
    unsigned int id = entity.get_id();
    if (!luaEntityRefs.count(id)) return;

    auto refIter = luaEntityRefs.at(id);
    removedEntities.push_back(id);

    luaEntities.erase(refIter);
    luaEntityRefs.erase(id);
}

std::list<Api::Usertype::Entity> &ServerDimension::getLuaEntities() {
    return luaEntities;
}

const std::list<unsigned int> &ServerDimension::getRemovedEntities() const {
    return removedEntities;
}

void ServerDimension::clearRemovedEntities() {
    removedEntities.clear();
}