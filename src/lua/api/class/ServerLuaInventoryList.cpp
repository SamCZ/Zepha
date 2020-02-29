//
// Created by aurailus on 2019-12-17.
//

#include "ServerLuaInventoryList.h"

int ServerLuaInventoryList::get_length() {
    return list.getLength();
}

int ServerLuaInventoryList::get_width() {
    return list.getWidth();
}

std::string ServerLuaInventoryList::get_name() {
    return list.getName();
}

LuaItemStack ServerLuaInventoryList::get_stack(unsigned short i) {
    return LuaItemStack(list.getStack(i - 1), list.defs);
}

void ServerLuaInventoryList::set_stack(unsigned short i, LuaItemStack stack) {
    list.setStack(i - 1, ItemStack(stack, list.defs));
}

void ServerLuaInventoryList::set_stack(unsigned short i, sol::table stack) {
    list.setStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack ServerLuaInventoryList::place_stack(unsigned short i, LuaItemStack stack) {
    return LuaItemStack(list.placeStack(i - 1, ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::place_stack(unsigned short i, sol::table stack) {
    return LuaItemStack(list.placeStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

LuaItemStack ServerLuaInventoryList::split_stack(unsigned short i) {
    return LuaItemStack(list.splitStack(i - 1), list.defs);
}

LuaItemStack ServerLuaInventoryList::add_stack(LuaItemStack stack) {
    return LuaItemStack(list.addStack(ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::add_stack(sol::table stack) {
    return LuaItemStack(list.addStack(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

int ServerLuaInventoryList::stack_fits(LuaItemStack stack) {
    return list.stackFits(ItemStack(stack, list.defs));
}

int ServerLuaInventoryList::stack_fits(sol::table stack) {
    return list.stackFits(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack ServerLuaInventoryList::take_stack(LuaItemStack request) {
    return LuaItemStack(list.takeStack(ItemStack(request, list.defs)), list.defs);
}

LuaItemStack ServerLuaInventoryList::take_stack(sol::table request) {
    return LuaItemStack(list.takeStack(ItemStack(list.defs.fromStr(request[1]).index, request.get<unsigned short>(2))), list.defs);
}

LuaItemStack ServerLuaInventoryList::remove_stack(unsigned short ind, unsigned short count) {
    return LuaItemStack(list.removeStack(ind - 1, count), list.defs);
}

void ServerLuaInventoryList::set_callback(ServerInventoryList::Callback t, sol::function fun) {
    list.setLuaCallback(t, fun);
}

sol::function ServerLuaInventoryList::get_callback(ServerInventoryList::Callback t) {
    return list.getLuaCallback(t);
}
