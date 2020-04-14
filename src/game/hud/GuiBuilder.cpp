//
// Created by aurailus on 2019-12-12.
//

#include "GuiBuilder.h"

#include "components/basic/GuiRect.h"
#include "components/basic/GuiText.h"
#include "components/basic/GuiModel.h"
#include "components/basic/GuiContainer.h"
#include "components/compound/GuiImageButton.h"

GuiBuilder::GuiBuilder(TextureAtlas& textures, ModelStore& models, std::shared_ptr<GuiContainer> root) :
        textures(textures), models(models), root(root) {}

void GuiBuilder::setGuiRoot(sol::state_view state, LuaGuiElement& menu) {
    elements = &menu;
}

void GuiBuilder::build(glm::ivec2 winBounds) {
    clear(false);
    if (elements) recursivelyCreate(*elements, root, winBounds);
}

void GuiBuilder::clear(bool deleteRoot) {
    recursivelyClearCallbacks(root);
    root->empty();
    if (deleteRoot) elements = nullptr;
}

void GuiBuilder::recursivelyCreate(const LuaGuiElement& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds) {
    auto component = createComponent(element, bounds);
    if (!component) throw std::runtime_error("GuiBuilder failed to create component: " + element.key);
    parent->add(component);

    for (auto& child : element.children) recursivelyCreate(child, component, component->getScale());
}

void GuiBuilder::recursivelyClearCallbacks(std::shared_ptr<GuiComponent> component) {
    component->setCallback(GuiComponent::CallbackType::PRIMARY, nullptr);
    component->setCallback(GuiComponent::CallbackType::SECONDARY, nullptr);
    component->setCallback(GuiComponent::CallbackType::HOVER, nullptr);

    for (auto& child : component->getChildren()) {
        recursivelyClearCallbacks(child);
    }
}

std::shared_ptr<GuiComponent> GuiBuilder::createComponent(const LuaGuiElement& elem, glm::ivec2 bounds) {
    std::shared_ptr<GuiComponent> c = nullptr;

    switch (Util::hash(elem.type.c_str())) {
        default: break;
        case Util::hash("Body"): {
            auto body = GuiRect::fromSerialized(elem, textures, bounds);
            body->setScale(bounds);
            c = body;
            break;
        }
        case Util::hash("Rect"):
            c = GuiRect::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("Button"):
            c = GuiImageButton::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("Text"):
            c = GuiText::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("Model"):
            c = GuiModel::fromSerialized(elem, textures, models, bounds);
            break;
    }

    if (!c) return nullptr;

    if (elem.callbacks.count("primary")) c->setCallback(GuiComponent::CallbackType::PRIMARY, [=](bool b, glm::vec2 v) {
        elem.callbacks.at("primary")(b, LuaParser::luaVec(elem.callbacks.at("primary").lua_state(), {v.x, v.y, 0})); });

    if (elem.callbacks.count("secondary")) c->setCallback(GuiComponent::CallbackType::SECONDARY, [=](bool b, glm::vec2 v) {
        elem.callbacks.at("secondary")(b, LuaParser::luaVec(elem.callbacks.at("secondary").lua_state(), {v.x, v.y, 0})); });

    if (elem.callbacks.count("hover")) c->setCallback(GuiComponent::CallbackType::HOVER, [=](bool b, glm::vec2 v) {
        elem.callbacks.at("hover")(b, LuaParser::luaVec(elem.callbacks.at("hover").lua_state(), {v.x, v.y, 0})); });

    return c;
}

GuiBuilder::~GuiBuilder() {
    clear();
}
