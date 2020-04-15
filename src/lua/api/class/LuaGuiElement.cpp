//
// Created by aurailus on 2020-04-12.
//

#include <glm/vec2.hpp>

#include "LuaGuiElement.h"

#include "../../../game/hud/SerialGui.h"

//LuaGuiElement::LuaGuiElement(const std::string& type, sol::table data) :
//        type(type) {
//
//    for (const auto& pair : data) {
//        if (pair.first.is<float>()) {
//            if (!pair.second.is<std::shared_ptr<LuaGuiElement>>()) throw std::runtime_error("Child is not a GuiElement.");
//            children.push_back(pair.second.as<std::shared_ptr<LuaGuiElement>>());
//        }
//        else if (pair.first.is<std::string>()) set_trait(pair.first.as<std::string>(), pair.second);
//    }
//}

std::shared_ptr<LuaGuiElement> LuaGuiElement::create(const std::string& type, sol::table data) {
    auto elem = std::make_shared<LuaGuiElement>();
    elem->type = type;

    for (const auto& pair : data) {
        if (pair.first.is<float>()) {
            if (!pair.second.is<std::shared_ptr<LuaGuiElement>>()) throw std::runtime_error("Child is not a GuiElement.");
            elem->children.push_back(pair.second.as<std::shared_ptr<LuaGuiElement>>());
        }
        else if (pair.first.is<std::string>()) elem->set_trait(pair.first.as<std::string>(), pair.second);
    }

    return elem;
}

sol::object LuaGuiElement::get_trait(sol::this_state s, const std::string& key) {
    if (key == "key") return sol::make_object<std::string>(s, this->key);
    if (key == "type") return sol::make_object<std::string>(s, this->type);

    if (traits.count(key)) return traits.at(key);
    return sol::nil;
}

sol::object LuaGuiElement::set_trait(const std::string& key, sol::object val) {
    if (key == "callbacks") {
        callbacks.clear();
        for (auto pair : val.as<sol::table>()) callbacks[pair.first.as<std::string>()] = pair.second.as<sol::function>();
    }
    else if (key == "key") {
        this->key = val.as<std::string>();
    }
    else {
        traits.erase(key);
        traits.emplace(key, val);
    }

    if (updateFunction) updateFunction();
    return val;
}

sol::object LuaGuiElement::call(sol::this_state s, sol::function fun) {
    sol::table tbl = sol::state_view(s)["zepha"]["__builtin"]["gui_env"];
    sol::environment env(s, sol::create, tbl);

    sol::set_environment(env, fun);
    return fun(this);
}

sol::object LuaGuiElement::get_child(sol::this_state s, sol::object key) {
    if (key.is<float>() && key.as<float>() <= children.size()) {
        auto begin = children.begin();
        std::advance(begin, key.as<float>() - 1);
        return sol::make_object<std::shared_ptr<LuaGuiElement>>(s, *begin);
    }
    else if (key.is<std::string>()) {
        for (auto &child : children) {
            if (child->key == key.as<std::string>()) return sol::make_object<std::shared_ptr<LuaGuiElement>>(s, child);
        }

        for (auto &child : children) {
            auto recurse = child->get_child(s, key);
            if (recurse) return recurse;
        }
    }

    return sol::nil;
}

void LuaGuiElement::append(sol::this_state s, sol::object elem) {
    if (elem.is<LuaGuiElement>()) children.push_back(elem.as<std::shared_ptr<LuaGuiElement>>());
    else if (elem.is<sol::function>()) children.push_back(call(s, elem.as<sol::function>()).as<std::shared_ptr<LuaGuiElement>>());
    else throw std::runtime_error("Append arg is not an element or a function to generate one.");

    if (updateFunction) updateFunction();
    children.back()->updateFunction = updateFunction;
}

void LuaGuiElement::prepend(sol::this_state s, sol::object elem) {
    if (elem.is<LuaGuiElement>()) children.insert(children.begin(), elem.as<std::shared_ptr<LuaGuiElement>>());
    else if (elem.is<sol::function>()) children.insert(children.begin(), call(s, elem.as<sol::function>()).as<std::shared_ptr<LuaGuiElement>>());
    else throw std::runtime_error("Append arg is not an element or a function to generate one.");

    if (updateFunction) updateFunction();
    children.front()->updateFunction = updateFunction;
}

void LuaGuiElement::remove(sol::optional<LuaGuiElement> elem) {
    if (!elem) {
        if (parent != nullptr) parent->remove(sol::make_optional<LuaGuiElement>(*this));
        else throw std::runtime_error("Tried to remove self from nil parent.");
    }
    else {
        for (const auto it = children.cbegin(); it != children.cend();) {
            if ((*it)->key == elem->key) {
                children.erase(it);
                if (updateFunction) updateFunction();
                return;
            }
        }
    }
}

Any LuaGuiElement::getAsAny(const std::string &key) const noexcept {
    if (!traits.count(key)) return Any();
    auto object = traits.at(key);

    if (object.is<std::string>()) return Any::from<std::string>(object.as<std::string>());
    else if (object.is<sol::table>()) {
        auto table = object.as<sol::table>();

        if (table.size() == 2) {
            auto x = table.get<sol::object>(1);
            auto y = table.get<sol::object>(2);

            glm::vec2 values = {};
            if (x.is<float>()) values.x = x.as<float>();
            else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
            if (y.is<float>()) values.y = y.as<float>();
            else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());

            return Any::from<glm::vec2>(values);
        }
        else if (table.size() == 4) {
            auto x = table.get<sol::object>(1);
            auto y = table.get<sol::object>(2);
            auto z = table.get<sol::object>(3);
            auto w = table.get<sol::object>(4);

            glm::vec4 values = {};
            if (x.is<float>()) values.x = x.as<float>();
            else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
            if (y.is<float>()) values.y = y.as<float>();
            else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());
            if (z.is<float>()) values.z = z.as<float>();
            else if (z.is<std::string>()) values.z = SerialGui::toDouble(z.as<std::string>());
            if (w.is<float>()) values.w = w.as<float>();
            else if (w.is<std::string>()) values.w = SerialGui::toDouble(w.as<std::string>());

            return Any::from<glm::vec4>(values);
        }
    }
}
