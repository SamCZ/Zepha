//
// Created by aurailus on 03/10/19.
//

#pragma once

#include "../class/LocalLuaEntity.h"
#include "../class/ServerLuaEntity.h"
#include "../../../def/ServerGame.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "../../../net/server/world/ServerWorld.h"
#include "../../../net/server/world/ServerEntity.h"

namespace Api {
    static int entities_ind = 0;

    static void add_entity_c(sol::state& lua, sol::table& core, ClientGame& defs, LocalWorld& world) {
        core["entities"] = lua.create_table();

        core.set_function("add_entity", [&](sol::optional<sol::table> pos, sol::optional<std::string> identifier, sol::object staticData) {
            if (!identifier || !identifier->length()) throw std::runtime_error("Expected an identifier as the second argument.");
            if (!pos) throw std::runtime_error("Expected a position as the first argument.");

            if (core["registered_entities"][*identifier] == sol::nil) throw std::runtime_error("identifier '" + *identifier + "' is not a valid entity identifier.");
            sol::table entityDef = core["registered_entities"][*identifier];

            auto entity = std::make_unique<Entity>();
            entity->setPos({pos->get<float>("x"), pos->get<float>("y"), pos->get<float>("z")});
            auto entityRef = std::make_shared<LocalLuaEntity>(std::move(entity), entities_ind++, defs);

            sol::table luaEntity = lua.create_table();
            luaEntity[sol::metatable_key] = entityDef;
            luaEntity["object"] = entityRef;
            luaEntity["name"] = identifier;

            auto displayType = luaEntity.get<sol::optional<std::string>>("display");
            auto displayObject = luaEntity.get<sol::optional<std::string>>("display_object");
            auto displayTexture = luaEntity.get<sol::optional<std::string>>("display_texture");

            if (!displayType) throw std::runtime_error("entity '" + *identifier + "' is missing the display property.");
            if (!displayObject) throw std::runtime_error("entity '" + *identifier + "' is missing the display_object property.");
            if (strncmp(displayType->data(), "model", 5) == 0 && !displayTexture) throw std::runtime_error("entity '" + *identifier + "' is missing the display_texture property.");

            entityRef->set_display_type(*displayType, *displayObject, displayTexture);

            core.get<sol::table>("entities")[entityRef->id] = luaEntity;
            auto on_create = entityDef.get<sol::optional<sol::function>>("on_create");
            if (on_create) (*on_create)(luaEntity, staticData);

            world.dimension.addLocalEntity(entityRef);
            return luaEntity;
        });
    }

    static void add_entity_s(sol::state& lua, sol::table& core, ServerGame& defs, ServerWorld& world) {
        core["entities"] = lua.create_table();

        core.set_function("add_entity", [&](sol::optional<sol::table> pos, sol::optional<std::string> identifier, sol::object staticData) {
            if (!identifier || !identifier->length()) throw std::runtime_error("Expected an identifier as the second argument.");
            if (!pos) throw std::runtime_error("Expected a position as the first argument.");

            if (core["registered_entities"][*identifier] == sol::nil) throw std::runtime_error("identifier '" + *identifier + "' is not a valid entity identifier.");
            sol::table entityDef = core["registered_entities"][*identifier];

            auto entity = std::make_unique<ServerEntity>(entities_ind);
            entity->setPos({pos->get<float>("x"), pos->get<float>("y"), pos->get<float>("z")});
            auto entityRef = std::make_shared<ServerLuaEntity>(std::move(entity), entities_ind++, defs);

            sol::table luaEntity = lua.create_table();
            luaEntity[sol::metatable_key] = entityDef;
            luaEntity["object"] = entityRef;
            luaEntity["name"] = identifier;

            auto displayType = luaEntity.get<sol::optional<std::string>>("display");
            auto displayObject = luaEntity.get<sol::optional<std::string>>("display_object");
            auto displayTexture = luaEntity.get<sol::optional<std::string>>("display_texture");

            if (!displayType) throw std::runtime_error("entity '" + *identifier + "' is missing the display property.");
            if (!displayObject) throw std::runtime_error("entity '" + *identifier + "' is missing the display_object property.");
            if (strncmp(displayType->data(), "model", 5) == 0 && !displayTexture) throw std::runtime_error("entity '" + *identifier + "' is missing the display_texture property.");

            entityRef->set_display_type(*displayType, *displayObject, displayTexture);

            core.get<sol::table>("entities")[entityRef->id] = luaEntity;
            auto on_create = entityDef.get<sol::optional<sol::function>>("on_create");
            if (on_create) (*on_create)(luaEntity, staticData);

            world.dimension.addLuaEntity(entityRef);
            return luaEntity;
        });
    }
}
