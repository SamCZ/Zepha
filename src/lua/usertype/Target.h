//
// Created by aurailus on 2020-07-27.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameUsertype.h"

#include "Dimension.h"
#include "../../world/Target.h"

namespace Api::Usertype {
    class Target : public SubgameUsertype {
    public:
        Target(const ::Target& target);

        std::string getType();

        ::Target::Type type;

        Dimension dim;
        glm::ivec3 pos;
        glm::ivec3 pos_above;

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}
