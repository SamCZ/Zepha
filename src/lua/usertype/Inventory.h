//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <memory>

#include "SubgameUsertype.h"

#include "../../util/CovariantPtr.h"

namespace Api::Usertype {
    class Inventory : public SubgameUsertype {
    public:
        Inventory(InventoryPtr inventory) : inventory(inventory) {}

        InventoryPtr inventory;

        sol::object add_list(sol::this_state s, std::string name, int size, int width);
        sol::object get_list(sol::this_state s, std::string name);
        void remove_list(std::string name);

        void set_default_list(sol::object list);
        sol::object get_default_list(sol::this_state s);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}