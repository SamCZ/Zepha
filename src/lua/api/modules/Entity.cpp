//
// Created by aurailus on 2020-07-25.
//

#include "Entity.h"

#include "../../Lua.h"
#include "../class/LocalLuaEntity.h"
#include "../class/ServerLuaEntity.h"
#include "../../../def/LocalSubgame.h"
#include "../../../def/ServerSubgame.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "../../../net/server/world/ServerWorld.h"

void Api::Module::Entity::bind() {
    core["entities"] = lua.create_table();

    core.set_function("add_entity", Util::bind_this(this, &Entity::addEntity));

    if (state == State::CLIENT) core.set_function("remove_entity", Util::bind_this(this, &Entity::clientRemoveEntity));
    else                        core.set_function("remove_entity", Util::bind_this(this, &Entity::serverRemoveEntity));
}

sol::object Api::Module::Entity::addEntity(glm::vec3 pos, const std::string &identifier, sol::optional<sol::object> staticData) {
    if (core["registered_entities"][identifier] == sol::nil) throw std::runtime_error(identifier + " is not a valid entity identifier.");
    sol::table def = core["registered_entities"][identifier];

    sol::table luaEntity = lua.create_table();
    luaEntity[sol::metatable_key] = def;
    luaEntity["name"] = identifier;

    auto displayType = luaEntity.get<sol::optional<std::string>>("display");
    if (!displayType) throw std::runtime_error("entity '" + identifier + "' is missing the display property.");

    auto displayObject = luaEntity.get<sol::optional<std::string>>("display_object");
    if (!displayObject) throw std::runtime_error("entity '" + identifier + "' is missing the display_object property.");

    auto displayTexture = luaEntity.get<sol::optional<std::string>>("display_texture");
    if (strncmp(displayType->data(), "model", 5) == 0 && !displayTexture) throw std::runtime_error("entity '" + identifier + "' is missing the display_texture property.");

    // Server
    if (state == State::SERVER) {
        auto entity = std::make_unique<ServerEntity>(ind);
        entity->setPos(pos);
        auto ref = std::make_shared<ServerLuaEntity>(std::move(entity), ind++, static_cast<ServerSubgame&>(game));

        luaEntity["object"] = ref;
        ref->set_display_type(*displayType, *displayObject, displayTexture);
        core.get<sol::table>("entities")[ref->id] = luaEntity;

        auto on_create = def.get<sol::optional<sol::protected_function>>("on_create");
        if (on_create) (*on_create)(luaEntity, staticData);

        static_cast<ServerWorld&>(world).dimension.addLuaEntity(ref);
    }
    // Client
    else {
        auto entity = std::make_unique<::Entity>();
        entity->setPos(pos);
        auto ref = std::make_shared<LocalLuaEntity>(std::move(entity), ind++, static_cast<LocalSubgame&>(game));

        luaEntity["object"] = ref;
        ref->set_display_type(*displayType, *displayObject, displayTexture);
        core.get<sol::table>("entities")[ref->id] = luaEntity;

        auto on_create = def.get<sol::optional<sol::protected_function>>("on_create");
        if (on_create) (*on_create)(luaEntity, staticData);

        static_cast<LocalWorld&>(world).dimension.addLocalEntity(ref);
    }

    return luaEntity;
}

void Api::Module::Entity::clientRemoveEntity(sol::table entity) {
    auto object = entity.get<sol::optional<std::shared_ptr<LocalLuaEntity>>>("object");
    if (!object) throw std::runtime_error("Attempting to rmeove an invalid entity.");

    sol::optional<sol::table> entityTable = core["entities"][(*object)->id];
    if (!entityTable) return;

    sol::optional<sol::protected_function> onDestroy = (*entityTable)["on_destroy"];
    if (onDestroy) (*onDestroy)();

    core["entities"][(*object)->id] = sol::nil;
    static_cast<LocalWorld&>(world).dimension.removeLocalEntity(*object);
}

void Api::Module::Entity::serverRemoveEntity(sol::table entity) {
    auto object = entity.get<sol::optional<std::shared_ptr<ServerLuaEntity>>>("object");
    if (!object) throw std::runtime_error("Attempting to rmeove an invalid entity.");

    sol::optional<sol::table> entityTable = core["entities"][(*object)->id];
    if (!entityTable) return;

    sol::optional<sol::protected_function> onDestroy = (*entityTable)["on_destroy"];
    if (onDestroy) (*onDestroy)();

    core["entities"][(*object)->id] = sol::nil;
    static_cast<ServerWorld&>(world).dimension.removeLuaEntity(*object);
}
