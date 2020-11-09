//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <map>
#include <string>
#include <memory>

#include "Inventory.h"
#include "ServerInventoryList.h"

#include "game/ServerSubgame.h"

class ServerClients;

class ServerInventory : public Inventory {
	public:
	ServerInventory(SubgamePtr game, const std::string& name, ServerClients& clients) :
		Inventory(game, name), clients(clients) {};
	
	virtual void createList(const std::string& name, unsigned short length, unsigned short width) override;
	
	void sendDirtyLists();
	
	private:
	ServerClients& clients;
};
