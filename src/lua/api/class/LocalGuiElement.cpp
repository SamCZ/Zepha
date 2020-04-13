//
// Created by aurailus on 2020-04-12.
//

#include "LocalGuiElement.h"

LocalGuiElement::LocalGuiElement(const std::string& type, sol::table data) :
    type(type) {

    for (const auto& pair : data) {
        if (pair.first.is<float>()) {
            if (!pair.second.is<LocalGuiElement>()) throw std::runtime_error("Child is not a GuiElement.");
            children.push_back(pair.second.as<LocalGuiElement>());
        }
        else if (pair.first.is<std::string>()) set(pair.first.as<std::string>(), pair.second);
    }
}

sol::object LocalGuiElement::get(sol::this_state s, const std::string& key) {
    if (key == "key") return sol::make_object<std::string>(s, this->key);
    if (key == "type") return sol::make_object<std::string>(s, this->type);

    if (traits.count(key)) return traits[key];
    return sol::nil;
}

sol::object LocalGuiElement::set(const std::string& key, sol::object val) {
    if (key == "callbacks") {
        callbacks.clear();
        for (auto pair : val.as<sol::table>()) callbacks[pair.first.as<std::string>()] = pair.second.as<sol::function>();
        return val;
    }

    if ((!traits.count(key) || traits.at(key) != val) && updateFunction) updateFunction(id);

    traits[key] = val;
    return val;
}

sol::object LocalGuiElement::call(sol::this_state s, sol::function fun) {
    sol::table tbl = sol::state_view(s)["zepha"]["__builtin"]["gui_env"];
    sol::environment env(s, sol::create, tbl);

    sol::set_environment(env, fun);
    return fun(this);
}

sol::object LocalGuiElement::find(sol::this_state s, const std::string& key) {
    for (auto& child : children) if (child.key == key) return sol::make_object<LocalGuiElement>(s, child);
    for (auto& child : children) {
        auto recurse = child.find(s, key);
        if (recurse) return recurse;
    }
    return sol::nil;
}

void LocalGuiElement::append(sol::this_state s, sol::object elem) {
    if (elem.is<LocalGuiElement>()) children.push_back(elem.as<LocalGuiElement>());
    else if (elem.is<sol::function>()) children.push_back(call(s, elem.as<sol::function>()).as<LocalGuiElement>());
    else throw std::runtime_error("Append arg is not an element or a function to generate one.");

    if (updateFunction) updateFunction(id);
}

void LocalGuiElement::prepend(sol::this_state s, sol::object elem) {
    if (elem.is<LocalGuiElement>()) children.insert(children.begin(), elem.as<LocalGuiElement>());
    else if (elem.is<sol::function>()) children.insert(children.begin(), call(s, elem.as<sol::function>()).as<LocalGuiElement>());
    else throw std::runtime_error("Append arg is not an element or a function to generate one.");

    if (updateFunction) updateFunction(id);
}

void LocalGuiElement::remove(sol::optional<LocalGuiElement> elem) {
    if (!elem) {
        if (parent != nullptr) parent->remove(sol::make_optional<LocalGuiElement>(*this));
        else throw std::runtime_error("Tried to remove self from nil parent.");
    }
    else {
        for (const auto it = children.cbegin(); it != children.cend();) {
            if (it->key == elem->key) {
                children.erase(it);
                if (updateFunction) updateFunction(id);
                return;
            }
        }
    }
}