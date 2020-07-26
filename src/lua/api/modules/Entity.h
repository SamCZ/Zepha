//
// Created by aurailus on 2020-07-25.
//

#pragma once

#include <glm/vec3.hpp>

#include "Module.h"

namespace Api::Module {
    class Entity : public Api::Module::Module {
    public:
        using Module::Module;
        void bind() override;

    protected:
        sol::object addEntity(glm::ivec3 pos, const std::string& identifier, sol::optional<sol::object> staticData);
        void clientRemoveEntity(sol::table entity);
        void serverRemoveEntity(sol::table entity);

        unsigned int ind = 0;
    };
}