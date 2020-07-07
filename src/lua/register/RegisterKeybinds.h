//
// Created by aurailus on 2020-01-10.
//

#pragma once

#include <sol2/sol.hpp>

#include "../../def/ClientGame.h"
#include "../../def/ServerGame.h"
#include "../../def/gen/BiomeDef.h"

namespace RegisterKeybinds {
    static void registerKeybinds(sol::table source, LuaKeybindHandler& keybinds) {

        // Register all of the items in the zepha.registered_keybinds table
        for (auto keybindRef : source) {

            // Basic properties
            std::string identifier = keybindRef.first.as<std::string>();

            if (!keybindRef.second || !keybindRef.second.is<sol::table>())
                throw identifier + "'s definition table is not a table!";
            sol::table keybindTbl = keybindRef.second.as<sol::table>();

            unsigned short def = keybindTbl.get<unsigned short>("default");

            auto onPress = keybindTbl.get<sol::optional<sol::function>>("on_press");
            auto onRelease = keybindTbl.get<sol::optional<sol::function>>("on_release");

            if (onPress) keybinds.bindOnDown(def, *onPress);
            if (onRelease) keybinds.bindOnUp(def, *onRelease);
        }
    }

//    static void server(sol::table& core, ServerGame& defs) {
//        registerItems(core.get<sol::table>("registered_items"), defs.defs, nullptr);
//    }

    static void client(sol::table& core, LuaKeybindHandler& manager) {
        registerKeybinds(core.get<sol::table>("registered_keybinds"), manager);
    }
};