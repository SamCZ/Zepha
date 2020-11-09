//
// Created by aurailus on 2020-02-26.
//

#pragma once

#include "InventoryRefs.h"

#include "ServerInventory.h"

class ServerClients;

class ServerInventoryList;

class ServerInventoryRefs : public InventoryRefs {
	public:
	ServerInventoryRefs(SubgamePtr game, ServerClients& clients);
	
	void update();
	
	virtual InventoryPtr createInventory(const std::string& inv) override;
	
	bool addWatcher(const std::string& inv, const std::string& list, unsigned int id);
	
	bool removeWatcher(const std::string& inv, const std::string& list, unsigned int id);
	
	bool interact(bool primary, const std::string& inv, const std::string& list, unsigned short ind, unsigned int id);
	
	private:
	ServerClients& clients;
};
