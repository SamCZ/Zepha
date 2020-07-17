//
// Created by aurailus on 2020-01-09.
//

#pragma once

namespace Api {
    static void register_entity(sol::state& lua, sol::table& core) {
        core["registered_entities"] = lua.create_table();

        core.set_function("register_entity", [&](sol::this_environment env, sol::optional<std::string> identifier, sol::optional<sol::table> data) {
            if (!identifier || !identifier->length()) throw std::runtime_error("expected a string as the first argument.");
            if (!data) throw std::runtime_error("expected a table as the second argument.");

            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");

            if (identifier->compare(0, modname.length() + 1, modname + ":")) throw std::runtime_error(
                        "identifier '" + *identifier + "' must begin with the calling mod's prefix, '" + modname + "'.");

            if (!data->get<sol::optional<std::string>>("display")) throw std::runtime_error("entity '" + *identifier + "' is missing the display property.");
            if (!data->get<sol::optional<std::string>>("display_object")) throw std::runtime_error("entity '" + *identifier + "' is missing the display_object property.");
            if (strncmp(data->get<std::string>("display_object").data(), "model", 5) == 0 && !data->get<sol::optional<std::string>>("display_texture"))
                throw std::runtime_error("entity '" + *identifier + "' is missing the display_texture property.");

            (*data)["__index"] = *data;
            core["registered_entities"][*identifier] = *data;
        });
    }
}