//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <memory>

#include "SubgameUsertype.h"

#include "ItemStack.h"
#include "../../util/CovariantPtr.h"

namespace Api::Usertype {
    class InventoryList : public SubgameUsertype {
    public:
        InventoryList(InventoryListPtr list) : list(list) {}

        InventoryListPtr list;

        void set_length(int length);
        int get_length();

        void set_width(int width);
        int get_width();

        void resize(int length, int width);

        std::string get_name();

        ItemStack get_stack(unsigned short i);

        void set_stack(unsigned short i, ItemStack stack);
        void set_stack(unsigned short i, sol::table stack);

        ItemStack place_stack(unsigned short i, ItemStack stack);
        ItemStack place_stack(unsigned short i, sol::table stack);

        ItemStack split_stack(unsigned short i);

        ItemStack add_stack(ItemStack stack);
        ItemStack add_stack(sol::table stack);

        int stack_fits(ItemStack stack);
        int stack_fits(sol::table stack);

        ItemStack take_stack(ItemStack request);
        ItemStack take_stack(sol::table request);

        ItemStack remove_stack(unsigned short ind, unsigned short count);

    //    void set_callback(ServerInventoryList::Callback t, sol::safe_function fun);
    //    sol::safe_function get_callback(ServerInventoryList::Callback t);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}