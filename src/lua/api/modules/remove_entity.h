//
// Created by aurailus on 11/10/19.
//

#pragma once

#include <sol2/sol.hpp>

#include "../class/LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"

namespace Api {
    static void remove_entity_c(sol::state& lua, sol::table& core, ClientGame& defs, LocalWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {
            auto object = entity.get<sol::optional<std::shared_ptr<LocalLuaEntity>>>("object");
            if (!object) throw "Attempting to remove an invalid entity object.";

            sol::optional<sol::table> luaEntTable = core["entities"][(*object)->id];
            if (!luaEntTable) return;

            sol::optional<sol::function> onDestruct = (*luaEntTable)["on_destroy"];
            if (onDestruct) (*onDestruct)();

            core["entities"][(*object)->id] = sol::nil;
            world.dimension.removeLocalEntity(*object);
        });
    }

    static void remove_entity_s(sol::state& lua, sol::table& core, ServerGame& defs, ServerWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {
            auto object = entity.get<sol::optional<std::shared_ptr<ServerLuaEntity>>>("object");
            if (!object) throw "Attempting to remove an invalid entity object.";

            sol::optional<sol::table> luaEntTable = core["entities"][(*object)->id];
            if (!luaEntTable) return;

            sol::optional<sol::function> onDestruct = (*luaEntTable)["on_destroy"];
            if (onDestruct) (*onDestruct)();

            core["entities"][(*object)->id] = sol::nil;
            // TODO: Tell the client when entities are deleted.
            world.dimension.removeLuaEntity(*object);
        });
    }
}