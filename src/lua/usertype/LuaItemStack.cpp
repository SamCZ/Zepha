//
// Created by aurailus on 2019-12-17.
//

#include "LuaItemStack.h"

#include "../Lua.h"
#include "../../def/ItemDef.h"
#include "../../def/Subgame.h"
#include "../../def/DefinitionAtlas.h"
#include "../../game/inventory/ItemStack.h"

LuaItemStack::LuaItemStack(const ItemStack &stack, Subgame& game) :
    name((stack.count == 0 ? "" : game.getDefs().fromId(stack.id).identifier)),
    count(stack.count) {}

LuaItemStack::LuaItemStack(const std::string& name, unsigned short count) :
    name(name), count(count) {}

LuaItemStack::LuaItemStack(sol::table tbl) :
    name(tbl.get<std::string>(1)), count(tbl.get<unsigned short>(2)) {}

std::string LuaItemStack::get_name() {
    return name;
}

void LuaItemStack::set_name(std::string name) {
    this->name = name;
}

int LuaItemStack::get_count() {
    return count;
}

void LuaItemStack::set_count(int count) {
    this->count = count;
}

bool LuaItemStack::is_empty() {
    return count == 0;
}
