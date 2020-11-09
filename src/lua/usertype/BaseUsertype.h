//
// Created by aurailus on 2020-07-27.
//

#pragma once

#include <sol/forward.hpp>

#include "../modules/BaseModule.h"

namespace Api {
	namespace Usertype {
		class BaseUsertype {
			public:
			BaseUsertype() = default;
			
			static void bind(State state, sol::state& lua, sol::table& core) {};
		};
	}
}