//
// Created by aurailus on 2019-12-12.
//

#include <array>

#include "GameGuiBuilder.h"

#include "components/compound/GuiInventoryList.h"

std::shared_ptr<GuiComponent> GameGuiBuilder::createComponent(LuaGuiElement& elem, glm::ivec2 bounds) {
    auto c = GuiBuilder::createComponent(elem, bounds);
    if (c != nullptr) return c;

    switch (Util::hash(elem.type.c_str())) {
        default: break;
        case Util::hash("InventoryList"): {
            c = GuiInventoryList::fromSerialized(elem, defs, bounds, refs);
            break;
        }
    }

    if (!c) return nullptr;

    elem.updateFunction = std::bind(&GameGuiBuilder::elementUpdated, this);

    if (elem.callbacks.count("primary")) c->setCallback(GuiComponent::CallbackType::PRIMARY, [=](bool b, glm::vec2 v) {
            elem.callbacks.at("primary")(b, LuaParser::luaVec(elem.callbacks.at("primary").lua_state(), {v.x, v.y, 0})); });

    if (elem.callbacks.count("secondary")) c->setCallback(GuiComponent::CallbackType::SECONDARY, [=](bool b, glm::vec2 v) {
            elem.callbacks.at("secondary")(b, LuaParser::luaVec(elem.callbacks.at("secondary").lua_state(), {v.x, v.y, 0})); });

    if (elem.callbacks.count("hover")) c->setCallback(GuiComponent::CallbackType::HOVER, [=](bool b, glm::vec2 v) {
            elem.callbacks.at("hover")(b, LuaParser::luaVec(elem.callbacks.at("hover").lua_state(), {v.x, v.y, 0})); });

    return c;
}
