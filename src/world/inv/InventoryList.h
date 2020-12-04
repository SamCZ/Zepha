//
// Created by aurailus on 2020-02-28.
//

#pragma once

#include <string>

#include "lua/Lua.h"
#include "game/Subgame.h"
#include "util/net/Packet.h"
#include "world/inv/ItemStack.h"

class DefinitionAtlas;

class InventoryList {
public:
//	enum class Callback { ALLOW_TAKE, ALLOW_PUT, ON_TAKE, ON_PUT };
	
	InventoryList(const InventoryList& o) = delete;
	
	InventoryList(SubgamePtr game, const std::string& name, const std::string& invName, unsigned short size,
		unsigned short width);
	
	std::string getName() const;
	
	void setWidth(unsigned short newWidth);
	
	unsigned short getWidth() const;
	
	void setLength(unsigned short length);
	
	unsigned short getLength() const;
	
	ItemStack getStack(unsigned short i) const;
	
	void setStack(unsigned short i, const ItemStack& stack);
	
	// Place the stack at i into the existing stack, returning overflow or other stack.
	virtual ItemStack placeStack(unsigned short i, const ItemStack& stack, bool playerInitiated = false);
	
	// Split the stack at i and return half of it, rounded up
	virtual ItemStack splitStack(unsigned short i, bool playerInitiated = false);
	
	// Adds as much of `stack` to the inventory as possible, returns leftover.
	virtual ItemStack addStack(ItemStack stack, bool playerInitiated = false);
	
	// Returns how many of `stack`'s items fit, from 0 to stack.count.
	virtual unsigned short stackFits(const ItemStack& stack);
	
	// Attempts to remove the usertype and amount of items specified by `stack`. Returns actual ItemStack of what was removed.
	virtual ItemStack takeStack(ItemStack request, bool playerInitiated = false);
	
	// Removes up to count items from ind, returns the items removed
	virtual ItemStack removeStack(unsigned short ind, unsigned short count);
	
	virtual void interact(InventoryListPtr cursor, bool primary, unsigned short ind);

//    sol::protected_function getLuaCallback(Callback type);
//    void setLuaCallback(Callback type, sol::protected_function cb);
	
	SubgamePtr getGame();
	
protected:
	SubgamePtr game;
	
	std::string name, invName;
	unsigned short width = 0;
	std::vector<ItemStack> items {};
	
	Packet createPacket();
	
	virtual void manipulated() = 0;
};
