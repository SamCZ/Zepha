#pragma once

#include "lua/Lua.h"

namespace MenuApi {
	void set_gui(sol::state& lua, sol::table& core, sptr<Gui::Element>& root) {
		core.set_function("set_gui", [&](sptr<Gui::Element> elem) {
			root->clear();
			root->append(elem);
		});
	}
}
