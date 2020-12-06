//
// Created by aurailus on 2020-07-27.
//

#pragma once

#include <glm/vec3.hpp>

#include "SubgameUsertype.h"

#include "Dimension.h"
#include "util/Target.h"

namespace Api::Usertype {
	class Target : public SubgameUsertype {
	public:
		Target(const ::Target& target);
		
		std::string type;
		std::optional<long long> id;
		std::optional<Dimension> dim;
		std::optional<glm::ivec3> pos;
		std::optional<glm::ivec3> pos_above;
		
		static void bind(State state, sol::state& lua, sol::table& core);
	};
}
