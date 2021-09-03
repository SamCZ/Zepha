#pragma once

#include <string>

#include "client/graph/Model.h"

/**
 * Base definition class, contains properties that all definition types use.
 * Inherited by BlockDef and CraftItemDef, which have specialized properties.
 */

class ItemDef {
public:
	enum class Type {
		INVALID,
		BLOCK,
		CRAFTITEM
	};
	
	ItemDef(const ItemDef& o) = delete;
	ItemDef(Type type): type(type) {}
	ItemDef(Type type, string identifier, string name, u16 index, u16 maxStack):
		type(type), identifier(identifier), name(name), index(index), maxStack(maxStack) {}
	
	string identifier {};
	string name {};
	u16 index = 0;
	
	u16 maxStack = 0;
	Type type = Type::INVALID;
	
	sptr<Model> entityModel = make_shared<Model>();
};

