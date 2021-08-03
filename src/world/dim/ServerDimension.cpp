//
// Created by aurailus on 01/10/19.
//

#include "ServerDimension.h"

#include "world/gen/MapGen.h"
#include "world/ServerWorld.h"
#include "game/ServerSubgame.h"
#include "lua/usertype/Player.h"
#include "lua/usertype/Target.h"
#include "server/ServerClient.h"
#include "server/ServerClients.h"
#include "world/dim/chunk/Chunk.h"
#include "world/dim/chunk/Region.h"
#include "world/dim/chunk/MapBlock.h"
#include "world/player/ServerPlayer.h"
#include "world/dim/ent/ServerLuaEntity.h"

ServerDimension::ServerDimension(SubgamePtr game, ServerWorld& world, const std::string& identifier,
	unsigned int ind, std::shared_ptr<MapGen> mapGen) :
	Dimension(game, static_cast<World&>(world), identifier, ind, std::move(mapGen)) {}

void ServerDimension::update(double delta) {
	
	/* Update server entities. */
	for (auto& entity : luaEntities) entity.entity.s()->update(delta);
	
	/*
	 * Delete mapblocks and regions that are outside of the retain range,
	 * and compress chunks if they are idle.
	 */
	
	for (let it = regions.cbegin(); it != regions.cend();) {
		for (u16 m = 0; m < 64; m++) {
			let mapBlock = it->second->get(m);
			if (!mapBlock) continue;
			
			bool clientNearby = false;
			for (auto& client : static_cast<ServerWorld&>(world).getClients().getClients()) {
				if (!client.second->player || client.second->player->getDim()->getInd() != ind) continue;
				auto clientMapBlock = Space::MapBlock::world::fromBlock(client.second->player->getPos());
				if (abs(clientMapBlock.x - mapBlock->pos.x) <= retainMapBlockRange.x &&
				    abs(clientMapBlock.y - mapBlock->pos.y) <= retainMapBlockRange.y &&
				    abs(clientMapBlock.z - mapBlock->pos.z) <= retainMapBlockRange.x) {
					clientNearby = true;
					break;
				}
			}
			
			if (!clientNearby) {
				it->second->remove(m);
				if (it->second->count <= 0) goto erase_region_and_continue;
			}
			else {
				for (u16 c = 0; c < 64; c++) {
					let chunk = mapBlock->get(c);
					if (chunk) chunk->compressIfIdle();
				}
			}
		}
		
		it++;
		continue;
		
		erase_region_and_continue:
		it = regions.erase(it);
	}
}

double ServerDimension::blockHit(const Target& target, PlayerPtr player) {
	double timeout = 0, damage = 0;
	sol::tie(damage, timeout) = game->getParser().safe_function(game->getParser().core["block_hit"],
		Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));
	
	return timeout;
}

void ServerDimension::blockPlace(const Target& target, PlayerPtr player) {
	std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
		game->getParser().core["block_place"], Api::Usertype::ServerPlayer(player.s()), Api::Usertype::Target(target));
	
	auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
	if (!stack) return;
	
	auto inv = player->getInventory();
	if (inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void ServerDimension::blockInteract(const Target& target, PlayerPtr player) {
	game->getParser().safe_function(game->getParser().core["block_interact"],
		Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));
}

void ServerDimension::blockPlaceOrInteract(const Target& target, PlayerPtr player) {
	std::tuple<sol::optional<Api::Usertype::ItemStack>, sol::optional<glm::vec3>> res = game->getParser().safe_function(
		game->getParser().core["block_interact_or_place"], Api::Usertype::ServerPlayer(player),
		Api::Usertype::Target(target));
	
	auto stack = std::get<sol::optional<Api::Usertype::ItemStack>>(res);
	if (!stack) return;
	
	auto inv = player.s()->getInventory();
	if (inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void ServerDimension::wieldItemUse(const Target& target, PlayerPtr player) {
	sol::optional<Api::Usertype::ItemStack> stack = game->getParser().safe_function(
		game->getParser().core["item_use"], Api::Usertype::ServerPlayer(player), Api::Usertype::Target(target));
	
	auto inv = player->getInventory();
	if (stack && inv->hasList(player->getWieldList()))
		inv->getList(player->getWieldList())->setStack(player->getWieldIndex(), ItemStack(*stack, game));
}

void ServerDimension::setChunk(std::shared_ptr<Chunk> chunk) {
	std::shared_ptr<Chunk> existing = getChunk(chunk->getPos());
	if (existing) chunk = combineChunks(chunk, existing);
	Dimension::setChunk(chunk);
}

i64 ServerDimension::nextEntityInd() {
	return entityInd++;
};

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

std::list<Api::Usertype::Entity>& ServerDimension::getLuaEntities() {
	return luaEntities;
}

const std::list<i64>& ServerDimension::getRemovedEntities() const {
	return removedEntities;
}

void ServerDimension::clearRemovedEntities() {
	removedEntities.clear();
}