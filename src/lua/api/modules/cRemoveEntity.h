//
// Created by aurailus on 11/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../type/LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"

namespace ClientApi {
    void remove_entity(sol::state& lua, sol::table& core, LocalDefs& defs, LocalWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {
            sptr<LocalLuaEntity> object = entity.get<sptr<LocalLuaEntity>>("object");
            sol::optional<sol::table> luaEntTable = core["entities"][object->id];
            if (!luaEntTable) return;
            sol::optional<sol::function> onDestruct = (*luaEntTable)["on_destroy"];
            if (onDestruct) (*onDestruct)();
            core["entities"][object->id] = sol::nil;
            world.dimension.removeLuaEntity(object);
        });
    }
}