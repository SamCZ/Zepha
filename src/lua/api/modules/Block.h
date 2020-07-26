//
// Created by aurailus on 2020-07-25.
//

#pragma once

#include <glm/vec3.hpp>

#include "Module.h"

namespace Api::Module {
    class Block : public Api::Module::Module {
    public:
        using Module::Module;
        void bind() override;

    protected:
        std::string getBlock(glm::ivec3 pos);
        void setBlock(glm::ivec3 pos, const std::string& identifier);
    };
}