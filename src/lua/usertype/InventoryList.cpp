//
// Created by aurailus on 2019-12-17.
//

#include "InventoryList.h"

#include "../../def/ItemDef.h"
#include "../../def/DefinitionAtlas.h"
#include "../../game/inventory/InventoryList.h"

void Api::Usertype::InventoryList::set_length(int length) {
    list->setLength(length);
}

int Api::Usertype::InventoryList::get_length() {
    return list->getLength();
}

void Api::Usertype::InventoryList::set_width(int width) {
    list->setWidth(width);
}

int Api::Usertype::InventoryList::get_width() {
    return list->getWidth();
}

void Api::Usertype::InventoryList::resize(int length, int width) {
    list->setLength(length);
    list->setWidth(width);
}

std::string Api::Usertype::InventoryList::get_name() {
    return list->getName();
}

LuaItemStack Api::Usertype::InventoryList::get_stack(unsigned short i) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list->getStack(i - 1), list->getGame());
}

void Api::Usertype::InventoryList::set_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    list->setStack(i - 1, ItemStack(stack, list->getGame()));
}

void Api::Usertype::InventoryList::set_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    list->setStack(i - 1, ItemStack(list->getGame().getDefs().fromStr(stack[1]).index, stack[2]));
}

LuaItemStack Api::Usertype::InventoryList::place_stack(unsigned short i, LuaItemStack stack) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list->placeStack(i - 1, ItemStack(stack, list->getGame())), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::place_stack(unsigned short i, sol::table stack) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list->placeStack(i - 1, ItemStack(list->getGame().getDefs().fromStr(stack[1]).index, stack[2])), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::split_stack(unsigned short i) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list->splitStack(i - 1), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::add_stack(LuaItemStack stack) {
    return LuaItemStack(list->addStack(ItemStack(stack, list->getGame())), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::add_stack(sol::table stack) {
    return LuaItemStack(list->addStack(ItemStack(list->getGame().getDefs().fromStr(stack[1]).index, stack[2])), list->getGame());
}

int Api::Usertype::InventoryList::stack_fits(LuaItemStack stack) {
    return list->stackFits(ItemStack(stack, list->getGame()));
}

int Api::Usertype::InventoryList::stack_fits(sol::table stack) {
    return list->stackFits(ItemStack(list->getGame().getDefs().fromStr(stack[1]).index, stack[2]));
}

LuaItemStack Api::Usertype::InventoryList::take_stack(LuaItemStack request) {
    return LuaItemStack(list->takeStack(ItemStack(request, list->getGame())), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::take_stack(sol::table request) {
    return LuaItemStack(list->takeStack(ItemStack(list->getGame().getDefs().fromStr(request[1]).index, request[2])), list->getGame());
}

LuaItemStack Api::Usertype::InventoryList::remove_stack(unsigned short i, unsigned short count) {
    if (i < 1 || i > list->getLength()) throw "index is outside of list bounds.";
    return LuaItemStack(list->removeStack(i - 1, count), list->getGame());
}

//void LuaInventoryList::set_callback(ServerInventoryList::Callback t, sol::safe_function fun) {
//    list->setLuaCallback(t, fun);
//}
//
//sol::safe_function LuaInventoryList::get_callback(ServerInventoryList::Callback t) {
//    return list->getLuaCallback(t);
//}

void Api::Usertype::InventoryList::bind(State state, sol::state &lua, sol::table &core) {
    if (state == State::SERVER) lua.new_usertype<InventoryList>("InventoryListRef",
        "length", sol::property(&InventoryList::get_length, &InventoryList::set_length),
        "width", sol::property(&InventoryList::get_width, &InventoryList::set_width),
        "name", sol::property(&InventoryList::get_name),

        "resize", &InventoryList::resize,

        "get_stack", &InventoryList::get_stack,
        "set_stack", sol::overload(
            sol::resolve<void(unsigned short, LuaItemStack)>(&InventoryList::set_stack),
            sol::resolve<void(unsigned short, sol::table)>(&InventoryList::set_stack)),
        "place_stack", sol::overload(
            sol::resolve<LuaItemStack(unsigned short, LuaItemStack)>(&InventoryList::place_stack),
            sol::resolve<LuaItemStack(unsigned short, sol::table)>(&InventoryList::place_stack)),
        "split_stack", &InventoryList::split_stack,
        "add_stack", sol::overload(
            sol::resolve<LuaItemStack(LuaItemStack)>(&InventoryList::add_stack),
            sol::resolve<LuaItemStack(sol::table)>(&InventoryList::add_stack)),
        "stack_fits", sol::overload(
            sol::resolve<int(LuaItemStack)>(&InventoryList::stack_fits),
            sol::resolve<int(sol::table)>(&InventoryList::stack_fits)),
        "take_stack", sol::overload(
            sol::resolve<LuaItemStack(LuaItemStack)>(&InventoryList::take_stack),
            sol::resolve<LuaItemStack(sol::table)>(&InventoryList::take_stack)),
        "remove_stack", &InventoryList::remove_stack
    );
    else lua.new_usertype<InventoryList>("InventoryListRef",
        "length", sol::property(&InventoryList::get_length),
        "width", sol::property(&InventoryList::get_width),
        "name", sol::property(&InventoryList::get_name),

        "get_stack", &InventoryList::get_stack,
        "set_stack", sol::overload(
            sol::resolve<void(unsigned short, LuaItemStack)>(&InventoryList::set_stack),
            sol::resolve<void(unsigned short, sol::table)>(&InventoryList::set_stack)),
        "place_stack", sol::overload(
            sol::resolve<LuaItemStack(unsigned short, LuaItemStack)>(&InventoryList::place_stack),
            sol::resolve<LuaItemStack(unsigned short, sol::table)>(&InventoryList::place_stack)),
        "split_stack", &InventoryList::split_stack,
        "add_stack", sol::overload(
            sol::resolve<LuaItemStack(LuaItemStack)>(&InventoryList::add_stack),
            sol::resolve<LuaItemStack(sol::table)>(&InventoryList::add_stack)),
        "stack_fits", sol::overload(
            sol::resolve<int(LuaItemStack)>(&InventoryList::stack_fits),
            sol::resolve<int(sol::table)>(&InventoryList::stack_fits)),
        "take_stack", sol::overload(
            sol::resolve<LuaItemStack(LuaItemStack)>(&InventoryList::take_stack),
            sol::resolve<LuaItemStack(sol::table)>(&InventoryList::take_stack)),
        "remove_stack", &InventoryList::remove_stack
    );
}