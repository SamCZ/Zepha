/** Pointers that can contain Server or Client versions of common CoVariant types. */

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
	std::shared_ptr<B> ptr = nullptr;
	
public:
	CovariantPtr() = default;
	
	CovariantPtr(std::nullptr_t) {};
	
	CovariantPtr(std::shared_ptr<B> ptr) : ptr(ptr) {}
	
	CovariantPtr(std::shared_ptr<L> ptr) : ptr(std::dynamic_pointer_cast<B>(ptr)) {}
	
	CovariantPtr(std::shared_ptr<S> ptr) : ptr(std::dynamic_pointer_cast<B>(ptr)) {}
	
	void operator=(std::nullptr_t) { ptr = nullptr; }
	
	operator bool() { return ptr != nullptr; }
	
	std::shared_ptr<B> operator*() { return ptr; }
	
	std::shared_ptr<B> operator->() { return ptr; }
	
	bool isL() const { return std::dynamic_pointer_cast<L>(ptr) != nullptr; }
	
	bool isS() const { return std::dynamic_pointer_cast<S>(ptr) != nullptr; }
	
	std::shared_ptr<L> l() {
		auto converted = std::dynamic_pointer_cast<L>(ptr);
		if (!converted) throw std::bad_cast();
		return converted;
	}
	
	std::shared_ptr<S> s() {
		auto converted = std::dynamic_pointer_cast<S>(ptr);
		if (!converted) throw std::bad_cast();
		return converted;
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