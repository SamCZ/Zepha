#pragma once

#include "lua/Lua.h"

#include "util/Types.h"

namespace Gui {
	class Root;
	class Props;
	class Element;
	enum class Prop;
	class Expression;
	class ExpressionInfo;
}

namespace Api::Usertype::GuiElement {
	Gui::Prop nameToProp(const string& str);
	any objectToProp(Gui::Prop prop, const sol::object& value);
	sol::object propToObject(const Gui::Props& props, Gui::Prop prop,
		const Gui::ExpressionInfo& expr, sol::state_view s);
	
	std::shared_ptr<Gui::Element> create(const string& type, sol::table data, Gui::Root& root);
	
	sol::object call(sol::this_state s, sol::protected_function fun);
	
	void bind(sol::state& lua, sol::table& core, Gui::Root& root);
}