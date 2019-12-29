//
// Created by aurailus on 03/10/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-static-cast-downcast"

#include "LocalLuaEntity.h"

void LocalLuaEntity::set_pos(const sol::table &pos) {
    entity->setPos({pos["x"], pos["y"], pos["z"]});
}

void LocalLuaEntity::int_pos(const sol::table &pos) {
    entity->interpPos({pos["x"], pos["y"], pos["z"]});
}

sol::table LocalLuaEntity::get_pos(sol::this_state s) {
    glm::vec3 pos = entity->getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

void LocalLuaEntity::set_visual_offset(const sol::table &vs) {
    entity->setVisualOffset({vs["x"], vs["y"], vs["z"]});
}

void LocalLuaEntity::int_visual_offset(const sol::table &vs) {
    entity->interpVisualOffset({vs["x"], vs["y"], vs["z"]});
}

sol::table LocalLuaEntity::get_visual_offset(sol::this_state s) {
    glm::vec3 v = entity->getVisualOffset();
    return sol::state_view(s).create_table_with("x", v.x, "y", v.y, "z", v.z);
}

void LocalLuaEntity::set_yaw(float rot) {
    entity->setAngle(rot);
}

void LocalLuaEntity::int_yaw(float rot) {
    entity->interpAngle(rot);
}

float LocalLuaEntity::get_yaw() {
    return entity->getAngle();
}

void LocalLuaEntity::set_scale(float scale) {
    entity->setScale(scale);
}

void LocalLuaEntity::int_scale(float scale) {
    entity->interpScale(scale);
}

float LocalLuaEntity::get_scale() {
    return entity->getScale();
}

void LocalLuaEntity::set_display_type(const std::string &type, const std::string &arg, sol::optional<std::string> arg2) {
    if (strncmp(type.data(), "gameobject", 10) == 0) {
        ItemDef& def = defs.defs.fromStr(arg);

        if (def.type == ItemDef::Type::BLOCK)
            entity->setModel(static_cast<BlockDef&>(def).entityModel);
        else if (def.type == ItemDef::Type::CRAFTITEM)
            entity->setModel(static_cast<CraftItemDef&>(def).entityModel);
    }
    else if (strncmp(type.data(), "model", 5) == 0 && arg2 && !arg2->empty()) {
        auto model = std::make_shared<Model>();
        model->fromSerialized(defs.models.models[arg], {defs.textures[*arg2]});
        entity->setModel(model);
    }
}

#pragma clang diagnostic pop