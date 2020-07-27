//
// Created by aurailus on 2020-07-25.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameModule.h"

namespace Api::Module {
    class Entity : public Api::Module::SubgameModule {
    public:
        using SubgameModule::SubgameModule;
        void bind() override;

    protected:
        sol::object addEntity(glm::vec3 pos, const std::string& identifier, sol::optional<sol::object> staticData);
        void clientRemoveEntity(sol::table entity);
        void serverRemoveEntity(sol::table entity);

        unsigned int ind = 0;
    };
}