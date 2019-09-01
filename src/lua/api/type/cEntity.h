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

        glm::vec3 get_pos(sol::this_state s) {
//            glm::vec3 pos = entity->getPos();
//            return sol::state_view(s).create_table_with("x", pos.x, "y", pos.y, "z", pos.z);
            return entity->getPos();
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

    void entity(sol::state& lua, sol::table &zeus, LocalDefs& defs, LocalWorld& world) {
        auto model = std::make_shared<Model>();
        model->import("/home/aurailus/Zepha/mods/default/models/rabbit.b3d", {defs.textures()["zeus:default:rabbit"]});

        lua.new_usertype<LuaEntity>(
                "LuaEntity",
                "new", sol::factories([&world, model, &lua](){
                    auto rabbit = std::make_shared<Entity>(model);
                    rabbit->setScale(1.f/16.f);
                    rabbit->setPos({0, 40, 0});

                    auto entity = LuaEntity {std::move(rabbit)};
                    world.addEntity(entity.entity);

                    return entity;
                }),
                "set_pos", &LuaEntity::set_pos,
                "get_pos", &LuaEntity::get_pos,
                "set_yaw", &LuaEntity::set_yaw,
                "get_yaw", &LuaEntity::get_yaw,
                "set_scale", &LuaEntity::set_scale,
                "get_scale", &LuaEntity::get_scale
        );
    }
}