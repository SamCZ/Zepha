//
// Created by aurailus on 2019-12-17.
//

#include "LuaInventoryList.h"

int LuaInventoryList::get_length() {
    return list.getLength();
}

int LuaInventoryList::get_width() {
    return list.getWidth();
}

LuaItemStack LuaInventoryList::get_stack(unsigned short i) {
    return LuaItemStack(list.getStack(i), list.defs);
}

void LuaInventoryList::set_stack(unsigned short i, LuaItemStack stack) {
    list.setStack(i, ItemStack(stack, list.defs));
}

void LuaInventoryList::set_table(unsigned short i, sol::table stack) {
    list.setStack(i, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack LuaInventoryList::place_stack(unsigned short i, LuaItemStack stack) {
    return LuaItemStack(list.placeStack(i, ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack LuaInventoryList::place_table(unsigned short i, sol::table stack) {
    return LuaItemStack(list.placeStack(i, ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

LuaItemStack LuaInventoryList::split_stack(unsigned short i) {
    return LuaItemStack(list.splitStack(i), list.defs);
}

LuaItemStack LuaInventoryList::add_stack(LuaItemStack stack) {
    return LuaItemStack(list.addStack(ItemStack(stack, list.defs)), list.defs);
}

LuaItemStack LuaInventoryList::add_table(sol::table stack) {
    return LuaItemStack(list.addStack(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2))), list.defs);
}

int LuaInventoryList::stack_fits(LuaItemStack stack) {
    return list.stackFits(ItemStack(stack, list.defs));
}

int LuaInventoryList::table_fits(sol::table stack) {
    return list.stackFits(ItemStack(list.defs.fromStr(stack[1]).index, stack.get<unsigned short>(2)));
}

LuaItemStack LuaInventoryList::remove_stack(LuaItemStack request) {
    return LuaItemStack(list.removeStack(ItemStack(request, list.defs)), list.defs);
}

LuaItemStack LuaInventoryList::remove_table(sol::table request) {
    return LuaItemStack(list.removeStack(ItemStack(list.defs.fromStr(request[1]).index, request.get<unsigned short>(2))), list.defs);
}
