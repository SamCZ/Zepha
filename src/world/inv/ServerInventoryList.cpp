//
// Created by aurailus on 2019-10-29.
//

#include "ServerInventoryList.h"

#include "util/net/Packet.h"
#include "server/ServerClient.h"
#include "server/ServerClients.h"
#include "lua/usertype/ItemStack.h"
#include "world/player/ServerPlayer.h"

ServerInventoryList::ServerInventoryList(SubgamePtr game, ServerClients& list,
	const std::string& name, const std::string& invName, unsigned short size, unsigned short width) :
	InventoryList(game, name, invName, size, width),
	clients(list) {}

void ServerInventoryList::manipulated() {
	dirty = true;
}

bool ServerInventoryList::addWatcher(unsigned int id) {
	if (watchers.count(id)) return false;
	auto& client = clients.getClient(id);
	if (!client || !client->player) return false;
	
	sendTo(client->player);
	watchers.insert(id);
	return true;
}

bool ServerInventoryList::removeWatcher(unsigned int id) {
	if (!watchers.count(id)) return false;
	watchers.erase(id);
	return true;
}

void ServerInventoryList::sendTo(std::shared_ptr<ServerPlayer> player) {
	if (!player) return;
	createPacket().sendTo(player->getPeer(), Packet::Channel::INTERACT);
}

void ServerInventoryList::sendToAll() {
	auto p = createPacket();
	
	for (auto it = watchers.cbegin(); it != watchers.cend();) {
		auto& client = clients.getClient(*it);
		if (!client || !client->player) it = watchers.erase(it);
		else {
			p.sendTo(client->peer, Packet::Channel::INTERACT);
			it++;
		}
	}
	
	dirty = false;
}
