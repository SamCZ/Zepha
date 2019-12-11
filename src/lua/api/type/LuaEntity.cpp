//
// Created by aurailus on 03/10/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-static-cast-downcast"

#include "LuaEntity.h"

void LuaEntity::set_pos(const sol::table &pos) {
    entity->setPos({pos["x"], pos["y"], pos["z"]});
}

void LuaEntity::int_pos(const sol::table &pos) {
    entity->interpPos({pos["x"], pos["y"], pos["z"]});
}

sol::table LuaEntity::get_pos(sol::this_state s) {
    glm::vec3 pos = entity->getPos();
    return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
}

void LuaEntity::set_visual_offset(const sol::table &vs) {
    entity->setVisualOffset({vs["x"], vs["y"], vs["z"]});
}

void LuaEntity::int_visual_offset(const sol::table &vs) {
    entity->interpVisualOffset({vs["x"], vs["y"], vs["z"]});
}

sol::table LuaEntity::get_visual_offset(sol::this_state s) {
    glm::vec3 v = entity->getVisualOffset();
    return sol::state_view(s).create_table_with("x", v.x, "y", v.y, "z", v.z);
}

void LuaEntity::set_yaw(float rot) {
    entity->setAngle(rot);
}

void LuaEntity::int_yaw(float rot) {
    entity->interpAngle(rot);
}

float LuaEntity::get_yaw() {
    return entity->getAngle();
}

void LuaEntity::set_scale(float scale) {
    entity->setScale(scale);
}

void LuaEntity::int_scale(float scale) {
    entity->interpScale(scale);
}

float LuaEntity::get_scale() {
    return entity->getScale();
}

void LuaEntity::set_display_type(const std::string &type, const std::string &arg, sol::optional<std::string> arg2) {
    if (type == std::string("gameobject")) {
        ItemDef& def = defs.defs.fromStr(arg);

        if (def.type == ItemDef::Type::BLOCK)
            entity->setModel(static_cast<BlockDef&>(def).entityModel);
        else if (def.type == ItemDef::Type::CRAFTITEM)
            entity->setModel(static_cast<CraftItemDef&>(def).entityModel);
    }
    else if (type == std::string("model") && arg2 && !arg2->empty()) {
        auto model = std::make_shared<Model>();
        model->fromSerialized(defs.models.models[arg], {defs.textures[*arg2]});
        entity->setModel(model);
    }
}

#pragma clang diagnostic pop