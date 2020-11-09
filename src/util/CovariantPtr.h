//
// Created by aurailus on 2020-08-02.
//

#pragma once

#include <memory>

class World;

class LocalWorld;

class ServerWorld;

class Player;

class LocalPlayer;

class ServerPlayer;

class LuaEntity;

class LocalLuaEntity;

class ServerLuaEntity;

class Subgame;

class LocalSubgame;

class ServerSubgame;

class Dimension;

class LocalDimension;

class ServerDimension;

class Inventory;

class LocalInventory;

class ServerInventory;

class InventoryList;

class LocalInventoryList;

class ServerInventoryList;

class InventoryRefs;

class LocalInventoryRefs;

class ServerInventoryRefs;

template<typename B, typename L, typename S>
class CovariantPtr {
	std::shared_ptr<B> b = nullptr;
	
	public:
	CovariantPtr() = default;
	
	CovariantPtr(std::nullptr_t) {};
	
	CovariantPtr(std::shared_ptr<B> ptr) : b(ptr) {}
	
	CovariantPtr(std::shared_ptr<L> ptr) : b(std::dynamic_pointer_cast<B>(ptr)) {}
	
	CovariantPtr(std::shared_ptr<S> ptr) : b(std::dynamic_pointer_cast<B>(ptr)) {}
	
	void operator=(std::nullptr_t) { b = nullptr; }
	
	operator bool() { return b != nullptr; }
	
	std::shared_ptr<B> operator*() { return b; }
	
	std::shared_ptr<B> operator->() { return b; }
	
	bool isL() const { return std::dynamic_pointer_cast<L>(b) != nullptr; }
	
	bool isS() const { return std::dynamic_pointer_cast<S>(b) != nullptr; }
	
	std::shared_ptr<L> l() {
		auto ptr = std::dynamic_pointer_cast<L>(b);
		if (!ptr) throw std::bad_cast();
		return ptr;
	}
	
	std::shared_ptr<S> s() {
		auto ptr = std::dynamic_pointer_cast<S>(b);
		if (!ptr) throw std::bad_cast();
		return ptr;
	}
};

typedef CovariantPtr<World, LocalWorld, ServerWorld> WorldPtr;
typedef CovariantPtr<Player, LocalPlayer, ServerPlayer> PlayerPtr;
typedef CovariantPtr<Subgame, LocalSubgame, ServerSubgame> SubgamePtr;
typedef CovariantPtr<LuaEntity, LocalLuaEntity, ServerLuaEntity> EntityPtr;
typedef CovariantPtr<Dimension, LocalDimension, ServerDimension> DimensionPtr;
typedef CovariantPtr<Inventory, LocalInventory, ServerInventory> InventoryPtr;
typedef CovariantPtr<InventoryList, LocalInventoryList, ServerInventoryList> InventoryListPtr;
typedef CovariantPtr<InventoryRefs, LocalInventoryRefs, ServerInventoryRefs> InventoryRefsPtr;