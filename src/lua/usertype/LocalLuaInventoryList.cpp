//
// Created by aurailus on 2020-02-24.
//

#include "LocalLuaInventoryList.h"

#include "LuaItemStack.h"
#include "../../def/ItemDef.h"
#include "../../def/DefinitionAtlas.h"

int LocalLuaInventoryList::get_length() {
    return list.getLength();
}

int LocalLuaInventoryList::get_width() {
    return list.getWidth();
}

std::string LocalLuaInventoryList::get_name() {
    return list.getName();
}

LuaItemStack LocalLuaInventoryList::get_stack(unsigned short i) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.getStack(i - 1), list.defs);
}

void LocalLuaInventoryList::set_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    list.setStack(i - 1, ItemStack(stack, list.defs));
}

void LocalLuaInventoryList::set_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    list.setStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack LocalLuaInventoryList::place_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.placeStack(i - 1, ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack LocalLuaInventoryList::place_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.placeStack(i - 1, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

LuaItemStack LocalLuaInventoryList::split_stack(unsigned short i) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.splitStack(i - 1), list.defs);
}

LuaItemStack LocalLuaInventoryList::add_stack(LuaItemStack stack) {
    return LuaItemStack(list.addStack(ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack LocalLuaInventoryList::add_stack(sol::table stack) {
    return LuaItemStack(list.addStack(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

int LocalLuaInventoryList::stack_fits(LuaItemStack stack) {
    return list.stackFits(ItemStack(stack, list.defs));
}

int LocalLuaInventoryList::stack_fits(sol::table stack) {
    return list.stackFits(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack LocalLuaInventoryList::take_stack(LuaItemStack request) {
    return LuaItemStack(list.takeStack(ItemStack(request, list.defs)), list.defs);
}

LuaItemStack LocalLuaInventoryList::take_stack(sol::table request) {
    return LuaItemStack(list.takeStack(ItemStack(list.defs.fromStr(request[1]).index, request.get<unsigned short>(2))), list.defs);
}

LuaItemStack LocalLuaInventoryList::remove_stack(unsigned short i, unsigned short count) {
    if (i < 1 || i > list.getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list.removeStack(i - 1, count), list.defs);
}

void LocalLuaInventoryList::set_callback(LocalInventoryList::Callback t, sol::protected_function fun) {
    list.setLuaCallback(t, fun);
}

sol::protected_function LocalLuaInventoryList::get_callback(LocalInventoryList::Callback t) {
    return list.getLuaCallback(t);
}
