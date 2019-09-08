//
// Created by aurailus on 30/08/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "../customization/pVec3.h"

#define SOL_ALL_SAFETIES_ON = 1
#include <sol2/sol.hpp>

namespace ClientApi {
    struct LuaEntity {
        std::shared_ptr<Entity> entity = nullptr;

        void set_pos(const sol::table& pos) {
            entity->interpPos({pos["x"], pos["y"], pos["z"]});
        }

        sol::table get_pos(sol::this_state s) {
            glm::vec3 pos = entity->getPos();
            return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
        }

        void set_visual_offset(const sol::table& vs) {
            entity->interpVisualOffset({vs["x"], vs["y"], vs["z"]});
        }

        sol::table get_visual_offset(sol::this_state s) {
            glm::vec3 v = entity->getVisualOffset();
            return sol::state_view(s).create_table_with("x", v.x, "y", v.y, "z", v.z);
        }

        void set_yaw(float rot) {
            entity->interpAngle(rot);
        }

        float get_yaw() {
            return entity->getAngle();
        }

        void set_scale(float scale) {
            entity->interpScale(scale);
        }

        float get_scale() {
            return entity->getScale();
        }
    };

    void entity(sol::state& lua, sol::table &core, LocalDefs& defs, LocalWorld& world) {
        lua.new_usertype<LuaEntity>(
                "LuaEntity",
                "new", sol::factories([&world, &defs, &lua](sol::table pos){
                    auto model = std::make_shared<Model>();
//                    model->import("/home/aurailus/Zepha/mods/default/models/rabbit.b3d", {defs.textures()["zeus:default:rabbit"]});
                    model->fromBlock(defs.defs().blockFromStr("default:sandstone"));

                    auto rabbit = std::make_shared<Entity>(model);
                    rabbit->setScale(1.0f/3.0f);
                    rabbit->setPos({pos.get<float>("x") + 0.5, pos.get<float>("y") + 0.5, pos.get<float>("z") + 0.5});

                    auto entity = LuaEntity {std::move(rabbit)};
                    world.addEntity(entity.entity);

                    return entity;
                }),
                "set_pos", &LuaEntity::set_pos,
                "get_pos", &LuaEntity::get_pos,
                "set_visual_offset", &LuaEntity::set_visual_offset,
                "get_visual_offset", &LuaEntity::get_visual_offset,
                "set_yaw", &LuaEntity::set_yaw,
                "get_yaw", &LuaEntity::get_yaw,
                "set_scale", &LuaEntity::set_scale,
                "get_scale", &LuaEntity::get_scale,
                "pos", sol::property(&LuaEntity::get_pos, &LuaEntity::set_pos),
                "visual_offset", sol::property(&LuaEntity::get_visual_offset, &LuaEntity::set_visual_offset),
                "yaw", sol::property(&LuaEntity::get_yaw, &LuaEntity::set_yaw),
                "scale", sol::property(&LuaEntity::get_scale, &LuaEntity::set_scale)
        );
    }
}