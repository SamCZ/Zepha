//
// Created by aurailus on 2020-04-12.
//

#pragma once

#include <list>
#include <sol2/sol.hpp>

class LocalGuiElement {
public:
    LocalGuiElement(const std::string& type, sol::table data);

    sol::object get(sol::this_state s, const std::string& key);
    sol::object set(const std::string& key, sol::object val);

    sol::object call(sol::this_state s, sol::function fun);
    sol::object find(sol::this_state s, const std::string& key);

    void append(sol::this_state s, sol::object elem);
    void prepend(sol::this_state s, sol::object elem);
    void remove(sol::optional<LocalGuiElement> elem);

    std::string type {}, key {};

    LocalGuiElement* parent = nullptr;
    std::list<LocalGuiElement> children {};

    std::unordered_map<std::string, sol::function> callbacks {};
    std::unordered_map<std::string, sol::object> traits {};

    // Internal data

    unsigned int id = 0;
    std::function<void(unsigned int id)> updateFunction = nullptr;
};