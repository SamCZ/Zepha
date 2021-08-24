#include "GuiElement.h"

#include "client/gui/Gui.h"
#include "client/gui/Root.h"
#include "client/gui/Element.h"
#include "client/gui/BoxElement.h"
#include "client/gui/TextElement.h"

static const Gui::Expression parseObjectToExpr(sol::object value) {
	if (!value.valid()) return Gui::Expression("");
	if (value.is<f32>()) return Gui::Expression(std::to_string(value.as<f32>()) + "dp");
	if (value.is<string>()) return Gui::Expression(value.as<string>());
	throw std::invalid_argument("Object cannot be converted to an expression.");
}

template <usize D>
static const array<Gui::Expression, D> parseLengthTableVal(sol::object value) {
	array<Gui::Expression, D> arr {};
	
	if (value.is<sol::table>()) {
		const let& t = value.as<sol::table>();
		
		vec<Gui::Expression> exprs {};
		exprs.reserve(t.size());
		for (usize i = 1; i <= t.size(); i++)
			exprs.emplace_back(parseObjectToExpr(t.get<sol::object>(i)));
		
		for (usize i = 0; i < arr.size() / exprs.size(); i++)
			for (usize j = 0; j < exprs.size(); j++)
				arr[j + i * exprs.size()] = exprs[j];
		
		return arr;
	}
	
	let v = parseObjectToExpr(value);
	for (usize i = 0; i < arr.size(); i++) arr[i] = v;
	return arr;
}

Gui::Prop Api::Usertype::GuiElement::nameToProp(const string& str) {
	using namespace Gui;
	
	const let ruleIt = Props::PROP_NAMES_TO_ENUM.find(str);
	if (ruleIt == Props::PROP_NAMES_TO_ENUM.end())
		throw std::invalid_argument("Style rule '" + str + "' doesn't exist.");
	return ruleIt->second;
}

any Api::Usertype::GuiElement::objectToProp(Gui::Prop prop, const sol::object& value) {
	using namespace Gui;
	
	switch (prop) {
	default:
		throw std::invalid_argument("Unhandled rule! This is an engine error! [1]");
	
	case Prop::ID:
	case Prop::LAYOUT:
	case Prop::DIRECTION:
	case Prop::H_ALIGN:
	case Prop::V_ALIGN:
	case Prop::CURSOR:
	case Prop::OVERFLOW:
	case Prop::TEXT_COLOR:
	case Prop::BACKGROUND:
	case Prop::BACKGROUND_HOVER:
	case Prop::CONTENT:
		return value.as<string>();
		
	case Prop::POS:
	case Prop::SIZE:
	case Prop::GAP:
		return parseLengthTableVal<2>(value);
		
	case Prop::MARGIN:
	case Prop::PADDING:
		return parseLengthTableVal<4>(value);
	
	case Prop::TEXT_SIZE:
		return parseObjectToExpr(value);
	}
}

sol::object Api::Usertype::GuiElement::propToObject(Gui::Prop prop, any value, sol::state_view s) {
	using namespace Gui;
	
	switch (prop) {
	default:
		throw std::invalid_argument("Unhandled rule! This is an engine error! [1]");
	
	case Prop::LAYOUT:
	case Prop::DIRECTION:
	case Prop::H_ALIGN:
	case Prop::V_ALIGN:
	case Prop::CURSOR:
	case Prop::OVERFLOW:
//	case Prop::TEXT_COLOR:
	case Prop::BACKGROUND:
	case Prop::BACKGROUND_HOVER:
	case Prop::CONTENT:
		return sol::make_object(s, std::any_cast<string>(value));
		
	case Prop::POS:
	case Prop::SIZE:
	case Prop::GAP:
	
//		return sol::make_object(s, s.create_table_with(
//
//		));
//		return sol::make_object(s, std::any_cast<string>(value));
		
	case Prop::MARGIN:
	case Prop::PADDING:
//		return parseLengthTableVal<4>(value);
	
	case Prop::TEXT_SIZE:
		break;
		throw std::invalid_argument("Unhandled rule! This is an engine error! [1]");
//		return parseObjectToExpr(value);
	}
}

sptr<Gui::Element> Api::Usertype::GuiElement::create(const string& type, sol::table data, Gui::Root& root) {
	Gui::Props props {};
	
	for (let& style : data) {
		if (!style.first.is<string>()) continue;
		let rule = GuiElement::nameToProp(style.first.as<string>());
		let value = GuiElement::objectToProp(rule, style.second);
		props.props[rule] = value;
	}
	
	sptr<Gui::Element> elem = nullptr;
	switch (Util::hash(type.data())) {
	default: throw std::invalid_argument("Invalid element type '" + type + "'.");
	case Util::hash("Box"): elem = root.create<Gui::BoxElement>(props); break;
	case Util::hash("Text"): elem = root.create<Gui::TextElement>(props); break;
	}
	
	for (usize i = 1; i <= data.size(); i++) {
		const sol::object& child = data.get<sol::object>(i);
		if (!child.is<sptr<Gui::Element>>()) continue;
		elem->append(child.as<sptr<Gui::Element>>());
	}
	
	return elem;
}

void Api::Usertype::GuiElement::bind(sol::state& lua, sol::table& core, Gui::Root& root) {
	lua.new_usertype<Gui::Element>("GuiElement",
		sol::meta_function::construct, sol::factories([&](const string& type, sol::table data) {
			return GuiElement::create(type, data, root);
		}),
		"prepend", [&](sol::this_state s, Gui::Element& self, sol::object child) {
			if (child.is<sptr<Gui::Element>>()) self.prepend(child.as<sptr<Gui::Element>>());
			else if (child.is<sol::protected_function>()) {
				sol::protected_function fn = child.as<sol::protected_function>();
				sol::table tbl = sol::state_view(s)["zepha"]["__builtin"]["gui_env"];
				sol::environment env(s, sol::create, tbl);
				sol::set_environment(env, fn);
				self.prepend(static_cast<sol::object>(fn()).as<sptr<Gui::Element>>());
			}
			else throw std::invalid_argument(
				"Cannot append() an item that is not an element or a function that returns one.");
		},
		"append", [&](sol::this_state s, Gui::Element& self, sol::object child) {
			if (child.is<sptr<Gui::Element>>()) self.append(child.as<sptr<Gui::Element>>());
			else if (child.is<sol::protected_function>()) {
				let fn = child.as<sol::protected_function>();
				sol::table tbl = sol::state_view(s)["zepha"]["__builtin"]["gui_env"];
				sol::environment env(s, sol::create, tbl);
				sol::set_environment(env, fn);
				self.append(static_cast<sol::object>(fn()).as<sptr<Gui::Element>>());
			}
			else throw std::invalid_argument(
				"append() parameter must be an element or a function that returns one.");
		},
		"get", [&](sol::this_state s, Gui::Element& self, sol::object query) -> sol::object {
			let found = query.is<u32>() ? self.get(query.as<u32>() - 1) : self.get(query.as<string>());
			if (!found) return sol::nil;
			return sol::make_object(s, found);
		},
		"remove", [&](sol::this_state s, Gui::Element& self, sol::object query) {
			if (query == sol::nil) self.remove();
			else if (query.is<u32>()) {
				let child = self.get(query.as<u32>());
				if (child) child->remove();
			}
			else if (query.is<std::string>()) {
				let child = self.get(query.as<std::string>());
				if (child) child->remove();
			}
			else if (query.is<sptr<Gui::Element>>()) {
				query.as<sptr<Gui::Element>>()->remove();
			}
			else throw std::invalid_argument(
				"remove() parameter must be nil, a number, an string, or a Gui element.");
		},
		"clear", &Gui::Element::clear,
		sol::meta_function::new_index, [&](Gui::Element& self, const string& ruleStr, sol::object rawValue) {
			let rule = GuiElement::nameToProp(ruleStr);
			let value = GuiElement::objectToProp(rule, rawValue);
			self.setProp(rule, value);
			self.updateElement();
		},
		sol::meta_function::index, [&](sol::this_state s, Gui::Element& self, const string& ruleStr) {
			let rule = GuiElement::nameToProp(ruleStr);
			let value = self.getProps().get(rule);
			if (!value) return sol::make_object(s, sol::nil);
			return GuiElement::propToObject(rule, *value, s);
		}
	);
}