//
// Created by aurailus on 11/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../game/entity/world/LuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"

namespace ClientApi {
    void remove_entity(sol::state& lua, sol::table& core, LocalDefs& defs, LocalWorld& world) {
        core.set_function("remove_entity", [&](sol::table entity) {
            sptr<LuaEntity> object = entity.get<sptr<LuaEntity>>("object");
            world.dimension.removeLuaEntity(object);
            core["__builtin_remove_entity"](object->id);
        });

        lua.script(R"(
            zepha.__builtin_remove_entity = function(id)
                for k, v in pairs(zepha.entities) do
                    if v.object.id == id then
                        zepha.registered_entities[k] = nil
                    end
                end
            end
        )");
    }
}