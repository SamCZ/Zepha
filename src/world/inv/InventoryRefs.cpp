//
// Created by aurailus on 2020-07-29.
//

#include "InventoryRefs.h"

#include "game/Subgame.h"

InventoryRefs::InventoryRefs(SubgamePtr game) : game(game) {}

bool InventoryRefs::hasInventory(const std::string& inv) {
	return inventories.count(inv);
}

InventoryPtr InventoryRefs::getInventory(const std::string& inv) {
	if (!inventories.count(inv)) throw std::runtime_error("Inventory " + inv + " doesn't exist~!");
	return inventories.at(inv);
}
