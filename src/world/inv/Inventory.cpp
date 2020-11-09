//
// Created by aurailus on 2020-07-29.
//

#include "Inventory.h"

#include "InventoryList.h"

Inventory::Inventory(SubgamePtr game, const std::string& name) : game(game), name(name) {}

bool Inventory::hasList(const std::string& name) {
	return lists.count(name);
}

void Inventory::removeList(const std::string& name) {
	lists.erase(name);
}

InventoryListPtr Inventory::getList(const std::string& name) {
	if (lists.count(name)) return InventoryListPtr(lists[name]);
	throw std::runtime_error("List " + name + " doesn't exist in inventory " + this->name + ".");
}