//
// Created by aurailus on 2020-07-28.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "SubgameUsertype.h"

class Dimension;

namespace Api::Usertype {
    class Dimension : public SubgameUsertype {
    public:
        Dimension(std::shared_ptr<::Dimension> dimension) : dimension(dimension) {}

        std::shared_ptr<::Dimension> dimension;

        std::string get_block(glm::ivec3 pos);
        void set_block(glm::ivec3 pos, const std::string& block);
        void remove_block(glm::ivec3 pos);

        double get_block_damage(glm::ivec3 pos);
        double set_block_damage(glm::ivec3 pos, double damage);
        double add_block_damage(glm::ivec3 pos, double damage);

        std::string get_biome(glm::ivec3 pos);
        void set_biome(glm::ivec3 pos, const std::string& biome);

        sol::table add_entity_c(sol::this_state s, glm::vec3 pos,
            const std::string &identifier, sol::optional<sol::object> staticData);
        sol::table add_entity_s(sol::this_state s, glm::vec3 pos,
            const std::string &identifier, sol::optional<sol::object> staticData);

        void remove_entity_c(sol::this_state s, sol::table entity);
        void remove_entity_s(sol::this_state s, sol::table entity);

        static void bind(State state, sol::state& lua, sol::table& core);
    };
}