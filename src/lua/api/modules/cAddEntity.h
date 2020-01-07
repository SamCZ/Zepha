//
// Created by aurailus on 03/10/19.
//

#pragma once

#include <sol2/sol.hpp>
#include "../type/LocalLuaEntity.h"
#include "../../../game/scene/world/LocalWorld.h"

namespace ClientApi {
    static int local_entities_ind = 0;

    void add_entity(sol::state& lua, sol::table& core, LocalDefs& defs, LocalWorld& world) {
        core["entities"] = lua.create_table();

        core.set_function("add_entity", [&](std::string entityStr, sol::table pos, sol::object staticData) {
            if (core["registered_entities"][entityStr] != sol::nil) {
                sol::table entityDef = core["registered_entities"][entityStr];

                auto entity = std::make_unique<Entity>();
                entity->setPos({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")});
                auto entityRef = std::make_shared<LocalLuaEntity>(std::move(entity), local_entities_ind++, defs);

                sol::table luaEntity = lua.create_table();
                luaEntity[sol::metatable_key] = entityDef;
                luaEntity["object"] = entityRef;
                luaEntity["name"] = entityStr;

                auto displayType = luaEntity.get<sol::optional<std::string>>("display");
                auto displayObject = luaEntity.get<sol::optional<std::string>>("display_object");
                auto displayTexture = luaEntity.get<sol::optional<std::string>>("display_texture");
                if (!displayType || !displayObject) throw "Missing display or display_object field.";
                if (*displayType == "model" && !displayTexture) throw "Missing model display_texture field.";

                entityRef->set_display_type(*displayType, *displayObject, displayTexture);

                core.get<sol::table>("entities")[entityRef->id] = luaEntity;
                auto on_create = entityDef.get<sol::optional<sol::function>>("on_create");
                if (on_create) (*on_create)(luaEntity, staticData);

                world.dimension.addLocalEntity(entityRef);
                return luaEntity;
            }
            throw "Tried to create undefined entity.";
        });
    }
}
