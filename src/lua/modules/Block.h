//
// Created by aurailus on 2020-07-25.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameModule.h"

namespace Api::Module {
    class Block : public Api::Module::SubgameModule {
    public:
        using SubgameModule::SubgameModule;
        void bind() override;

    protected:
        std::string getBlock(glm::ivec3 pos);
        void setBlock(glm::ivec3 pos, const std::string& identifier);

        double damageGet(glm::ivec3 pos);
        double damageSet(glm::ivec3 pos, double damage);
        double damageAdd(glm::ivec3 pos, double damage);
    };
}