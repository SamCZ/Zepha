//
// Created by aurailus on 2020-03-15.
//

#pragma once

#include "util/Schematic.h"

namespace Api {
	static void create_structure(sol::state& lua, sol::table& core) {
		core.set_function("create_structure", [&](sol::optional<sol::table> data) {
			if (!data) throw "expected a table as the first argument.";
			
			auto origin = data->get<sol::optional<glm::vec3>>("origin");
			auto layout = data->get < sol::optional < sol::table >> ("layout");
			
			if (!origin) throw std::runtime_error("expected a table as the first argument.");
			if (!layout) throw std::runtime_error("expected a table as the first argument.");
			
			auto s = std::make_shared<Schematic>();
			
			unsigned int yWid = layout->size();
			unsigned int zWid = (*layout).get<sol::table>(1).size();
			unsigned int xWid = (*layout).get<sol::table>(1).get<sol::table>(1).size();
			
			s->dimensions = { xWid, yWid, zWid };
			s->stringData.resize(xWid * yWid * zWid);
			
			s->origin = origin ? glm::ivec3{ *origin } : glm::ivec3{};
			
			for (unsigned int y = 1; y <= yWid; y++) {
				for (unsigned int z = 1; z <= zWid; z++) {
					for (unsigned int x = 1; x <= xWid; x++) {
						s->stringData[s->index({ x - 1, y - 1, z - 1 })] =
							layout->get<sol::table>(y).get<sol::table>(z).get_or<std::string>(x, "");
					}
				}
			}
			
			return s;
		});
	}
}
