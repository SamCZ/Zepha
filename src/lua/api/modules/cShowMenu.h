//
// Created by aurailus on 01/08/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include "../../../game/hud/GameGui.h"
#include "../../../game/hud/components/GUIComponent.h"
#include "../../../game/hud/components/basic/GUIContainer.h"
#include "../../../game/hud/components/basic/GUIText.h"
#include "../../../game/hud/components/basic/GUIRect.h"
#include <sol2/sol.hpp>

const static float scale_modifier = 3;

float calcNum(std::string input) {
    if (input.find("px") != string::npos) {
        return atof(input.substr(0, input.find("px")).c_str());
    }
}

glm::vec2 calc2dVec(std::string input) {
    glm::vec2 r;
    size_t count = std::count(input.begin(), input.end(), ',');

    if (count == 0) { //1 value
        r.x = calcNum(input);
        r.y = r.x;
        return r;
    }

    if (count == 1) { //2 values
        size_t delim = input.find(',');
        std::string x = input.substr(0, delim);
        std::string y = input.substr(delim + 1);

        r.x = calcNum(x);
        r.y = calcNum(y);
        return r;
    }

    std::cout << Log::err << "Invalid dimension string " << input << Log::endl;
    return r;
}

glm::vec4 calc4dVec(std::string input) {
    glm::vec4 r;
    size_t count = std::count(input.begin(), input.end(), ',');

    if (count == 0) { //1 value
        r = glm::vec4(calcNum(input));
        return r;
    }

    if (count == 1) { //2 values
        size_t delim = input.find(',');
        std::string x = input.substr(0, delim);
        std::string y = input.substr(delim + 1);

        r.x = calcNum(x);
        r.y = calcNum(y);
        r.z = r.x;
        r.w = r.y;
        return r;
    }

    if (count == 3) { //4 values
        size_t begin = 0;
        for (int i = 0; i < 3; i++) {
            size_t end = input.find(',', begin);
            r[i] = calcNum(input.substr(begin, end - begin));
            begin = end + 1;
        }
        r[3] = calcNum(input.substr(begin));
        return r;
    }

    std::cout << Log::err << "Invalid dimension string " << input << Log::endl;
    return r;
}

void processElement(LocalDefs& defs, sol::table& element, std::shared_ptr<GUIComponent> parent) {
    auto key  = element.get<std::string>("key");
    auto type = element.get<std::string>("type");

    std::shared_ptr<GUIComponent> component;

    if (type == "rect") {
        std::string posStr = element.get<std::string>("position");
        std::string sizeStr = element.get<std::string>("size");
        std::string padStr = element.get<std::string>("padding");

        glm::vec2 pos = calc2dVec(posStr) * scale_modifier;
        glm::vec2 size = calc2dVec(sizeStr) * scale_modifier;
        glm::vec4 padding = calc4dVec(padStr) * scale_modifier;
        std::string background = element.get_or<std::string>("background", "#0000");

        std::shared_ptr<GUIRect> rect = std::make_shared<GUIRect>(key);

        if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
        else rect->create(size - glm::vec2{padding.y + padding.w, padding.x + padding.z}, padding, defs.textures().getTextureRef(background));

        component = rect;
        rect->setPos(pos);
    }
    else if (type == "text") {
        std::string posStr = element.get<std::string>("position");
        std::string sizeStr = element.get<std::string>("size");
        std::string padStr = element.get<std::string>("padding");

        glm::vec2 pos = calc2dVec(posStr) * scale_modifier;
        glm::vec2 scale = glm::vec2(scale_modifier);
        glm::vec4 padding = calc4dVec(padStr) * scale_modifier;
        glm::vec4 bgcolor = Util::hexToColorVec(element.get_or<std::string>("background_color", "#0000"));
        glm::vec4 color = Util::hexToColorVec(element.get_or<std::string>("color", "#fff"));
        std::string contents = element.get<std::string>("contents");

        std::shared_ptr<GUIText> text = std::make_shared<GUIText>(key);
        text->create(scale, padding, bgcolor, color, {defs.textures(), defs.textures().getTextureRef("font")});
        text->setText(contents);
        component = text;
        text->setPos(pos);
    }

    parent->add(component);

    auto children = element.get<sol::optional<sol::table>>("children");
    if (children) {
        for (auto &child : *children) {
            sol::table elem = child.second;
            processElement(defs, elem, component);
        }
    }
}

namespace ClientApi {
    void show_menu(sol::table &core, LocalDefs& defs, Player& player) {

        core.set_function("show_menu", [&](std::string state, sol::table gui_root) {
            auto root = std::make_shared<GUIContainer>("__lua_root");
            player.setMenu(state, root);

            processElement(defs, gui_root, static_pointer_cast<GUIComponent>(root));
        });
    }
}

