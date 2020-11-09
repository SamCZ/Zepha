//
// Created by aurailus on 2019-12-17.
//

#include "ItemStack.h"

#include "lua/Lua.h"
#include "game/Subgame.h"
#include "game/def/ItemDef.h"
#include "world/inv/ItemStack.h"
#include "game/atlas/DefinitionAtlas.h"

Api::Usertype::ItemStack::ItemStack(const ::ItemStack& stack, SubgamePtr game) :
    name((stack.count == 0 ? "" : game->getDefs().fromId(stack.id).identifier)),
    count(stack.count) {}

Api::Usertype::ItemStack::ItemStack(const std::string& name, unsigned short count) :
    name(name), count(count) {}

Api::Usertype::ItemStack::ItemStack(sol::table tbl) :
    name(tbl.get<std::string>(1)), count(tbl.get<unsigned short>(2)) {}

std::string Api::Usertype::ItemStack::get_name() {
    return name;
}

void Api::Usertype::ItemStack::set_name(const std::string& newName) {
    name = newName;
}

int Api::Usertype::ItemStack::get_count() {
    return count;
}

void Api::Usertype::ItemStack::set_count(int newCount) {
    count = newCount;
}

bool Api::Usertype::ItemStack::is_empty() {
    return count == 0;
}

void Api::Usertype::ItemStack::bind(State state, sol::state &lua, sol::table &core) {
    lua.new_usertype<ItemStack>("ItemStack",
        sol::constructors<ItemStack(sol::table), ItemStack(std::string, unsigned short)>(),

        "name", sol::property(&ItemStack::get_name, &ItemStack::set_name),
        "count", sol::property(&ItemStack::get_count, &ItemStack::set_count),

        "is_empty", &ItemStack::is_empty
    );
}