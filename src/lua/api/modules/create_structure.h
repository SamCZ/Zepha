//
// Created by aurailus on 2020-03-15.
//

#pragma once

#include "../../../game/scene/world/Schematic.h"

namespace Api {
    static void create_structure(sol::state& lua, sol::table& core) {
        core.set_function("create_structure", [&](sol::optional<sol::table> data) {
            if (!data) throw "expected a table as the first argument.";

            auto origin = data->get<sol::optional<glm::vec3>>("origin");
            auto schematic = data->get<sol::optional<sol::table>>("schematic");

            if (!origin) throw std::runtime_error("expected a table as the first argument.");
            if (!schematic) throw std::runtime_error("expected a table as the first argument.");

            auto s = std::make_shared<Schematic>();

            unsigned int yWid = schematic->size();
            unsigned int zWid = (*schematic).get<sol::table>(1).size();
            unsigned int xWid = (*schematic).get<sol::table>(1).get<sol::table>(1).size();

            s->dimensions = {xWid, yWid, zWid};
            s->stringData.resize(xWid * yWid * zWid);

            s->origin = origin ? glm::ivec3{*origin} : glm::ivec3 {};

            for (unsigned int y = 1; y <= yWid; y++) {
                for (unsigned int z = 1; z <= zWid; z++) {
                    for (unsigned int x = 1; x <= xWid; x++) {
                        s->stringData[s->index({x - 1, y - 1, z - 1})] =
                            schematic->get<sol::table>(y).get<sol::table>(z).get_or<std::string>(x, "");
                    }
                }
            }

            return s;
        });
    }
}
