//
// Created by aurailus on 2020-04-12.
//

#pragma once

#include <sol2/sol.hpp>
#include "../class/LocalGuiElement.h"

namespace ClientApi {
    static void gui_element(sol::state& lua) {
        lua.new_usertype<LocalGuiElement>("GuiElement",
            sol::constructors<LocalGuiElement(std::string, sol::object)>(),

            sol::meta_function::index, &LocalGuiElement::get,
            sol::meta_function::new_index, &LocalGuiElement::set,

            sol::meta_function::call, &LocalGuiElement::call,

            "find", &LocalGuiElement::find,
            "append", &LocalGuiElement::append,
            "prepend", &LocalGuiElement::prepend,
            "remove", &LocalGuiElement::remove
        );
    }
}
