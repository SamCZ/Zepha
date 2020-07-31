//
// Created by aurailus on 2020-07-28.
//

#include "Dimension.h"

#include "../Lua.h"
#include "../LuaParser.h"
#include "LocalLuaEntity.h"
#include "../../def/gen/BiomeDef.h"
#include "../../def/item/BlockDef.h"
#include "../../world/LocalDimension.h"
#include "../../net/server/world/ServerEntity.h"
#include "../../def/ServerSubgame.h"
#include "ServerLuaEntity.h"
#include "../../world/ServerDimension.h"

std::string Api::Usertype::Dimension::get_block(glm::ivec3 pos) {
    return dimension->getGame().getDefs().fromId(dimension->getBlock(pos)).identifier;
}

void Api::Usertype::Dimension::set_block(glm::ivec3 pos, const std::string &block) {
    dimension->setBlock(pos, dimension->getGame().getDefs().fromStr(block).index);
}

void Api::Usertype::Dimension::remove_block(glm::ivec3 pos) {
    dimension->setBlock(pos, DefinitionAtlas::AIR);
}

double Api::Usertype::Dimension::get_block_damage(glm::ivec3 pos) {
    return dimension->getBlockDamage(pos);
}

double Api::Usertype::Dimension::set_block_damage(glm::ivec3 pos, double damage) {
    return dimension->setBlockDamage(pos, damage);
}

double Api::Usertype::Dimension::add_block_damage(glm::ivec3 pos, double damage) {
    return dimension->setBlockDamage(pos, dimension->getBlockDamage(pos) + damage);
}

std::string Api::Usertype::Dimension::get_biome(glm::ivec3 pos) {
    return dimension->getGame().getBiomes().biomeFromId(dimension->getBiome(pos)).identifier;
}

void Api::Usertype::Dimension::set_biome(glm::ivec3 pos, const std::string &biome) {
    dimension->setBiome(pos, dimension->getGame().getBiomes().biomeFromStr(biome).index);
}

sol::table Api::Usertype::Dimension::add_entity_c(sol::this_state s, glm::vec3 pos,
    const std::string &identifier, sol::optional<sol::object> staticData) {
    sol::state_view lua = sol::state_view(s);
    sol::table core = lua.get<sol::table>("zepha");

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

    auto entity = std::make_unique<::DrawableEntity>();
    entity->setPos(pos);
    auto ref = std::make_shared<LocalLuaEntity>(std::move(entity), dimension->nextEntityInd(), static_cast<LocalSubgame&>(dimension->getGame()));

    luaEntity["object"] = ref;
    ref->set_display_type(*displayType, *displayObject, displayTexture);
    core.get<sol::table>("entities")[ref->id] = luaEntity;

    auto on_create = def.get<sol::optional<sol::protected_function>>("on_create");
    if (on_create) (*on_create)(luaEntity, staticData);

    std::static_pointer_cast<LocalDimension>(dimension)->addLocalEntity(ref);
    return luaEntity;
}

sol::table Api::Usertype::Dimension::add_entity_s(sol::this_state s, glm::vec3 pos,
    const std::string &identifier, sol::optional<sol::object> staticData) {
    sol::state_view lua = sol::state_view(s);
    sol::table core = lua.get<sol::table>("zepha");

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

    unsigned int ind = dimension->nextEntityInd();
    auto entity = std::make_unique<ServerEntity>(ind);
    entity->setPos(pos);
    auto ref = std::make_shared<ServerLuaEntity>(std::move(entity), ind, static_cast<ServerSubgame&>(dimension->getGame()));

    luaEntity["object"] = ref;
    ref->set_display_type(*displayType, *displayObject, displayTexture);
    core.get<sol::table>("entities")[ref->id] = luaEntity;

    auto on_create = def.get<sol::optional<sol::protected_function>>("on_create");
    if (on_create) (*on_create)(luaEntity, staticData);

    std::static_pointer_cast<ServerDimension>(dimension)->addLuaEntity(ref);
    return luaEntity;
}

void Api::Usertype::Dimension::remove_entity_c(sol::this_state s, sol::table entity) {
    sol::state_view lua = sol::state_view(s);
    sol::table core = lua.get<sol::table>("zepha");

    auto object = entity.get<sol::optional<std::shared_ptr<LocalLuaEntity>>>("object");
    if (!object) throw std::runtime_error("Attempting to remove an invalid entity.");

    sol::optional<sol::table> entityTable = core["entities"][(*object)->id];
    if (!entityTable) return;

    sol::optional<sol::protected_function> onDestroy = (*entityTable)["on_destroy"];
    if (onDestroy) (*onDestroy)();

    core["entities"][(*object)->id] = sol::nil;
    std::static_pointer_cast<LocalDimension>(dimension)->removeLocalEntity(*object);
}

void Api::Usertype::Dimension::remove_entity_s(sol::this_state s, sol::table entity) {
    sol::state_view lua = sol::state_view(s);
    sol::table core = lua.get<sol::table>("zepha");

    auto object = entity.get<sol::optional<std::shared_ptr<ServerLuaEntity>>>("object");
    if (!object) throw std::runtime_error("Attempting to rmeove an invalid entity.");

    sol::optional<sol::table> entityTable = core["entities"][(*object)->id];
    if (!entityTable) return;

    sol::optional<sol::protected_function> onDestroy = (*entityTable)["on_destroy"];
    if (onDestroy) (*onDestroy)();

    core["entities"][(*object)->id] = sol::nil;
    std::static_pointer_cast<ServerDimension>(dimension)->removeLuaEntity(*object);
}

void Api::Usertype::Dimension::bind(State state, sol::state &lua, sol::table &core) {
    lua.new_usertype<Dimension>("LocalDimension",
        "get_block", &Dimension::get_block,
        "set_block", &Dimension::set_block,
        "remove_block", &Dimension::remove_block,

        "get_block_damage", &Dimension::get_block_damage,
        "set_block_damage", &Dimension::set_block_damage,
        "add_block_damage", &Dimension::add_block_damage,

        "get_biome", &Dimension::get_block,
        "set_biome", &Dimension::set_block,

        "add_entity", (state == State::CLIENT ? &Dimension::add_entity_c : &Dimension::add_entity_s),
        "remove_entity", (state == State::CLIENT ? &Dimension::remove_entity_c : &Dimension::remove_entity_s)
    );
}
