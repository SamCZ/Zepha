#pragma once

#include "lua/Lua.h"

#include "util/Types.h"

namespace Gui {
	class Style;
	class Root;
	class Element;
	enum class Prop;
}

namespace Api::Usertype::GuiElement {
	Gui::Prop nameToProp(const string& str);
	any objectToProp(Gui::Prop prop, const sol::object& value);
	sol::object propToObject(Gui::Prop prop, any value, sol::state_view s);
	
	std::shared_ptr<Gui::Element> create(const string& type, sol::table data, Gui::Root& root);
	
	sol::object call(sol::this_state s, sol::protected_function fun);
	
	void bind(sol::state& lua, sol::table& core, Gui::Root& root);
}
//class LuaGuiElement {
//	public:
//	LuaGuiElement() = default;
//
//	// Lua Functions and Properties
//
//	sol::object get_trait(sol::this_state s, const std::string& key);
//
//	sol::object set_trait(const std::string& key, sol::object val);
//
//
//	sol::object get_child(sol::this_state s, sol::object key);
//
//	void append(sol::this_state s, sol::object elem);
//
//	void prepend(sol::this_state s, sol::object elem);
//
//	void remove(sol::this_state s, sol::object elem);
//
//	void clear(sol::this_state s);
//
//	std::string type{}, key{};
//
//	LuaGuiElement* parent = nullptr;
//	std::list<std::shared_ptr<LuaGuiElement>> children{};
//
//	std::unordered_map<std::string, sol::function> callbacks{};
//	std::unordered_map<std::string, sol::object> traits{};
//
//	// C++ Functions and Properties
//	Any getAsAny(const std::string& key) const;
//
//	template<typename T>
//	const T& get(const std::string& key) const {
//		return getAsAny(key).get<T>();
//	}
//
//	template<typename T>
//	const T& get_or(const std::string& key, const T& other) const noexcept {
//		Any a = getAsAny(key);
//		if (a.empty() || !a.is<T>()) return other;
//		return a.get<T>();
//	}
//
//	template<typename T>
//	const bool has(const std::string& key) const noexcept {
//		Any a = getAsAny(key);
//		return !a.empty() && a.is<T>();
//	}
//
//	std::function<void()> updateFunction = nullptr;
//};
//
//namespace ClientApi {
//	static void gui_element(sol::state& lua) {
//		lua.new_usertype<Gui::Element>("GuiElement",
//			sol::meta_function::construct, sol::factories(&LuaGuiElement::create),
//
//			sol::meta_function::index, &LuaGuiElement::get_trait,
//			sol::meta_function::new_index, &LuaGuiElement::set_trait,
//
//			sol::meta_function::call, &LuaGuiElement::call,
//
//			"get", &LuaGuiElement::get_child,
//			"append", &LuaGuiElement::append,
//			"prepend", &LuaGuiElement::prepend,
//			"remove", &LuaGuiElement::remove,
//			"clear", &LuaGuiElement::clear
//		);
//	}
//}
