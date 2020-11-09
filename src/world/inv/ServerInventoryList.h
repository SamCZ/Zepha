//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <set>

#include "InventoryList.h"

class ServerClients;

class ServerPlayer;

class Packet;

class ServerInventoryList : public InventoryList {
	public:
	ServerInventoryList(SubgamePtr defs, ServerClients& list, const std::string& name,
		const std::string& invName, unsigned short size, unsigned short width);
	
	bool addWatcher(unsigned int id);
	
	bool removeWatcher(unsigned int id);
	
	void sendToAll();
	
	void sendTo(std::shared_ptr<ServerPlayer> player);
	
	bool dirty = false;
	
	private:
	void manipulated() override;
	
	ServerClients& clients;
	std::set<unsigned int> watchers{};
};
