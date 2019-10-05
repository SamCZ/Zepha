//
// Created by aurailus on 03/10/19.
//

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

void LuaEntity::set_display_type(const std::string &type, const std::string &arg) {
    if (type == std::string("gameobject")) {
        ItemDef& def = defs.defs().fromStr(arg);
        auto model = std::make_shared<Model>();

        switch (def.type) {
            case ItemDef::Type::BLOCK:
                model->fromBlock(static_cast<BlockDef&>(def));
                break;
            case ItemDef::Type::CRAFTITEM:
                model->fromItem(static_cast<CraftItemDef&>(def), defs.textures());
                break;
            default:
                throw "Invalid definition type.";
        }

        entity->setModel(model);
    }
}