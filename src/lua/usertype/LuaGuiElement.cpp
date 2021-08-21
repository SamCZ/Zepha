#include "LuaGuiElement.h"

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
	
	throw std::invalid_argument("Invalid length table configuration.");
}

void Api::Usertype::GuiElement::parseRule(const string& ruleStr, const sol::object& value, Gui::Style& styles) {
	using namespace Gui;
	
	const let ruleIt = Style::RULE_STRINGS_TO_ENUMS.find(ruleStr);
	if (ruleIt == Style::RULE_STRINGS_TO_ENUMS.end())
		throw std::invalid_argument("Style rule '" + ruleStr + "' doesn't exist.");
	const StyleRule rule = ruleIt->second;
	
	switch (rule) {
	default:
		throw std::invalid_argument("Unhandled rule '" + ruleStr + "'! This is an engine error!");
	
	case StyleRule::LAYOUT:
	case StyleRule::DIRECTION:
	case StyleRule::H_ALIGN:
	case StyleRule::V_ALIGN:
	case StyleRule::CURSOR:
	case StyleRule::OVERFLOW:
	case StyleRule::TEXT_COLOR:
	case StyleRule::BACKGROUND:
	case StyleRule::BACKGROUND_HOVER:
	case StyleRule::CONTENT:
		styles.rules[rule] = value.as<string>();
		break;
		
	case StyleRule::POS:
	case StyleRule::SIZE:
	case StyleRule::GAP:
		styles.rules[rule] = parseLengthTableVal<2>(value);
		break;
		
	case StyleRule::MARGIN:
	case StyleRule::PADDING:
		styles.rules[rule] = parseLengthTableVal<4>(value);
		break;
	
	case StyleRule::TEXT_SIZE:
		styles.rules[rule] = parseObjectToExpr(value);
		break;
	}
}

std::shared_ptr<Gui::Element> Api::Usertype::GuiElement::create(const string& type, sol::table data, Gui::Root& root) {
	Gui::Element::Props props {};
	
	for (let& style : data) {
		if (!style.first.is<string>()) continue;
		GuiElement::parseRule(style.first.as<string>(), style.second, props.styles);
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
	
//	let elem = root.create<Gui::BoxElement>(props);
//	return elem;
	//	auto elem = std::make_shared<LuaGuiElement>();
//
//	elem->type = type;
//
//	for (const auto& pair : data) {
//		if (pair.first.is<float>()) {
//			if (!pair.second.is<std::shared_ptr<LuaGuiElement>>()) continue;
//			elem->children.push_back(pair.second.as<std::shared_ptr<LuaGuiElement>>());
//			elem->children.back()->parent = elem.get();
//		}
//		else if (pair.first.is<std::string>()) elem->set_trait(pair.first.as<std::string>(), pair.second);
//	}
//
//	return elem;
}

void Api::Usertype::GuiElement::bind(sol::state& lua, sol::table& core, Gui::Root& root) {
	lua.new_usertype<Gui::Element>("GuiElement",
		sol::meta_function::construct, sol::factories([&](const string& type, sol::table data) {
			return GuiElement::create(type, data, root);
		})
	);
}

//
////		sol::meta_function::index, &LuaGuiElement::get_trait,
////		sol::meta_function::new_index, &LuaGuiElement::set_trait,
//
////		sol::meta_function::call, &LuaGuiElement::call,
//
////		"get", &LuaGuiElement::get_child,
////		"append", &LuaGuiElement::append,
////		"prepend", &LuaGuiElement::prepend,
////		"remove", &LuaGuiElement::remove,
////		"clear", &LuaGuiElement::clear


//
//sol::object LuaGuiElement::get_trait(sol::this_state s, const std::string& key) {
//	if (key == "key") return sol::make_object<std::string>(s, this->key);
//	if (key == "type") return sol::make_object<std::string>(s, this->type);
//
//	if (traits.count(key)) return traits.at(key);
//	return sol::nil;
//}
//
//sol::object LuaGuiElement::set_trait(const std::string& key, sol::object val) {
//	if (key == "callbacks") {
//		callbacks.clear();
//		for (auto pair : val.as<sol::table>()) callbacks[pair.first.as<std::string>()] = pair.second.as<sol::function>();
//	}
//	else if (key == "key") {
//		this->key = val.as<std::string>();
//	}
//	else {
//		traits.erase(key);
//		traits.emplace(key, val);
//	}
//
//	if (updateFunction) updateFunction();
//	return val;
//}
//
//sol::object LuaGuiElement::call(sol::this_state s, sol::protected_function fun) {
//	sol::table tbl = sol::state_view(s)["zepha"]["__builtin"]["gui_env"];
//	sol::environment env(s, sol::create, tbl);
//
//	sol::set_environment(env, fun);
//	return fun(this);
//}
//
//sol::object LuaGuiElement::get_child(sol::this_state s, sol::object key) {
//	if (key.is<float>() && key.as<float>() <= children.size()) {
//		auto begin = children.begin();
//		std::advance(begin, key.as<float>() - 1);
//		return sol::make_object<std::shared_ptr<LuaGuiElement>>(s, *begin);
//	}
//	else if (key.is<std::string>()) {
//		for (auto& child : children) {
//			if (child->key == key.as<std::string>()) return sol::make_object<std::shared_ptr<LuaGuiElement>>(s, child);
//		}
//
//		for (auto& child : children) {
//			auto recurse = child->get_child(s, key);
//			if (recurse) return recurse;
//		}
//	}
//
//	return sol::nil;
//}
//
//void LuaGuiElement::append(sol::this_state s, sol::object elem) {
//	if (elem.is<std::shared_ptr<LuaGuiElement>>()) children.push_back(elem.as<std::shared_ptr<LuaGuiElement>>());
//	else if (elem.is<sol::protected_function>())
//		children.push_back(call(s, elem.as<sol::protected_function>()).as<std::shared_ptr<LuaGuiElement>>());
//	else throw std::runtime_error("Append arg is not an element or a function to generate one.");
//
//	children.back()->parent = this;
//	if (updateFunction) updateFunction();
//}
//
//void LuaGuiElement::prepend(sol::this_state s, sol::object elem) {
//	if (elem.is<std::shared_ptr<LuaGuiElement>>())
//		children.insert(children.begin(), elem.as<std::shared_ptr<LuaGuiElement>>());
//	else if (elem.is<sol::function>())
//		children.insert(children.begin(), call(s, elem.as<sol::function>()).as<std::shared_ptr<LuaGuiElement>>());
//	else throw std::runtime_error("Append arg is not an element or a function to generate one.");
//
//	children.front()->parent = this;
//	if (updateFunction) updateFunction();
//}
//
//void LuaGuiElement::remove(sol::this_state s, sol::object elem) {
//	if (!elem) {
//		if (parent != nullptr) parent->remove(s, sol::make_object<std::string>(s, key));
//		else throw std::runtime_error("Tried to remove self from nil parent.");
//	}
//	else if (elem.is<std::string>()) {
//		auto child = this->get_child(s, sol::make_object<std::string>(s, elem.as<std::string>()));
//		if (child) remove(s, child);
//	}
//	else if (elem.is<std::shared_ptr<LuaGuiElement>>()) {
//		auto parent = elem.as<std::shared_ptr<LuaGuiElement>>()->parent;
//
//		for (auto it = parent->children.cbegin(); it != parent->children.cend(); it++) {
//			if ((*it)->key == elem.as<std::shared_ptr<LuaGuiElement>>()->key) {
//				(*it)->parent = nullptr;
//				(*it)->updateFunction = nullptr;
//
//				parent->children.erase(it);
//				if (parent->updateFunction) parent->updateFunction();
//				return;
//			}
//		}
//	}
//}
//
//void LuaGuiElement::clear(sol::this_state s) {
//	for (auto it = children.cbegin(); it != children.cend();) {
//		(*it)->parent = nullptr;
//		(*it)->updateFunction = nullptr;
//		it = children.erase(it);
//	}
//
//	if (updateFunction) updateFunction();
//}
//
//Any LuaGuiElement::getAsAny(const std::string& key) const {
//	if (!traits.count(key)) return Any();
////	auto object = traits.at(key);
////
////	if (object.is<float>()) return Any::from<float>(object.as<float>());
////	else if (object.is<bool>()) return Any::from<bool>(object.as<bool>());
////	else if (object.is<std::string>()) return Any::from<std::string>(object.as<std::string>());
////	else if (object.is<sol::table>()) {
////		auto table = object.as<sol::table>();
////
////		if (table.size() == 2) {
////			auto x = table.get<sol::object>(1);
////			auto y = table.get<sol::object>(2);
////
////			glm::vec2 values = {};
////			if (x.is<float>()) values.x = x.as<float>();
////			else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
////			if (y.is<float>()) values.y = y.as<float>();
////			else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());
////
////			return Any::from<glm::vec2>(values);
////		}
////		else if (table.size() == 4) {
////			auto x = table.get<sol::object>(1);
////			auto y = table.get<sol::object>(2);
////			auto z = table.get<sol::object>(3);
////			auto w = table.get<sol::object>(4);
////
////			glm::vec4 values = {};
////			if (x.is<float>()) values.x = x.as<float>();
////			else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
////			if (y.is<float>()) values.y = y.as<float>();
////			else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());
////			if (z.is<float>()) values.z = z.as<float>();
////			else if (z.is<std::string>()) values.z = SerialGui::toDouble(z.as<std::string>());
////			if (w.is<float>()) values.w = w.as<float>();
////			else if (w.is<std::string>()) values.w = SerialGui::toDouble(w.as<std::string>());
////
////			return Any::from<glm::vec4>(values);
////		}
////	}
//
//	throw std::runtime_error("Invalid type requested in getAsAny");
//}