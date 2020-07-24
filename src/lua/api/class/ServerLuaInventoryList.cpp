//
// Created by aurailus on 2019-12-17.
//

#include "ServerLuaInventoryList.h"

#include "../../../def/ItemDef.h"
#include "../../../def/DefinitionAtlas.h"

void ServerLuaInventoryList::set_length(int length) {
    list.setLength(length);
}

int ServerLuaInventoryList::get_length() {
    return list.getLength();
}

void ServerLuaInventoryList::set_width(int width) {
    list.setWidth(width);
}

int ServerLuaInventoryList::get_width() {
    return list.getWidth();
}

void ServerLuaInventoryList::resize(int length, int width) {
    list.setLength(length);
    list.setWidth(width);
}

std::string ServerLuaInventoryList::get_name() {
    return list.getName();
}

LuaItemStack ServerLuaInventoryList::get_stack(unsigned short i) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.getStack(i - 1), list.defs);
}

void ServerLuaInventoryList::set_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    list.setStack(i - 1, ItemStack(stack, list.defs));
}

void ServerLuaInventoryList::set_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    list.setStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack[2]));
}

LuaItemStack ServerLuaInventoryList::place_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.placeStack(i - 1, ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::place_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.placeStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack[2])), list.defs);
}

LuaItemStack ServerLuaInventoryList::split_stack(unsigned short i) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.splitStack(i - 1), list.defs);
}

LuaItemStack ServerLuaInventoryList::add_stack(LuaItemStack stack) {
    return LuaItemStack(list.addStack(ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::add_stack(sol::table stack) {
    return LuaItemStack(list.addStack(ItemStack(list.defs.fromStr(stack[1]).index, stack[2])), list.defs);
}

int ServerLuaInventoryList::stack_fits(LuaItemStack stack) {
    return list.stackFits(ItemStack(stack, list.defs));
}

int ServerLuaInventoryList::stack_fits(sol::table stack) {
    return list.stackFits(ItemStack(list.defs.fromStr(stack[1]).index, stack[2]));
}

LuaItemStack ServerLuaInventoryList::take_stack(LuaItemStack request) {
    return LuaItemStack(list.takeStack(ItemStack(request, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::take_stack(sol::table request) {
    return LuaItemStack(list.takeStack(ItemStack(list.defs.fromStr(request[1]).index, request[2])), list.defs);
}

LuaItemStack ServerLuaInventoryList::remove_stack(unsigned short i, unsigned short count) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.removeStack(i - 1, count), list.defs);
}

void ServerLuaInventoryList::set_callback(ServerInventoryList::Callback t, sol::safe_function fun) {
    list.setLuaCallback(t, fun);
}

sol::safe_function ServerLuaInventoryList::get_callback(ServerInventoryList::Callback t) {
    return list.getLuaCallback(t);
}