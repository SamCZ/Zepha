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

void GuiBuilder::setGuiTable(sol::state_view state, sol::table menu) {
    keyInd = 0;
    serialized = rDeserialize(state, menu);
}

SerialGui::Element GuiBuilder::rDeserialize(sol::state_view state, sol::table menu) {
    std::string type = menu.get<std::string>("type");
    std::string key = (menu.get<sol::optional<std::string>>("key") ? menu.get<std::string>("key") : "__UNKEYED_COMPONENT_" + std::to_string(keyInd++));

    SerialGui::Element element(type, key);

    for (auto trait : menu.get<sol::table>("traits")) {
        auto key = trait.first.as<std::string>();
        if (trait.second.is<std::string>()) {
            element.addTrait(key, Any::from<std::string>(trait.second.as<std::string>()));
        }
        else if (trait.second.is<sol::table>()) {
            if (trait.second.as<sol::table>().size() == 2) {
                auto x = trait.second.as<sol::table>().get<sol::object>(1);
                auto y = trait.second.as<sol::table>().get<sol::object>(2);

                glm::vec2 values = {};
                if (x.is<float>()) values.x = x.as<float>();
                else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
                if (y.is<float>()) values.y = y.as<float>();
                else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());

                element.addTrait(key, Any::from<glm::vec2>(values));
            }
            else if (trait.second.as<sol::table>().size() == 4) {
                auto x = trait.second.as<sol::table>().get<sol::object>(1);
                auto y = trait.second.as<sol::table>().get<sol::object>(2);
                auto z = trait.second.as<sol::table>().get<sol::object>(3);
                auto w = trait.second.as<sol::table>().get<sol::object>(4);

                glm::vec4 values = {};
                if (x.is<float>()) values.x = x.as<float>();
                else if (x.is<std::string>()) values.x = SerialGui::toDouble(x.as<std::string>());
                if (y.is<float>()) values.y = y.as<float>();
                else if (y.is<std::string>()) values.y = SerialGui::toDouble(y.as<std::string>());
                if (z.is<float>()) values.z = z.as<float>();
                else if (z.is<std::string>()) values.z = SerialGui::toDouble(z.as<std::string>());
                if (w.is<float>()) values.w = w.as<float>();
                else if (w.is<std::string>()) values.w = SerialGui::toDouble(w.as<std::string>());

                element.addTrait(key, Any::from<glm::vec4>(values));
            }
        }
    }

    auto callbacks = menu.get<sol::optional<sol::table>>("callbacks");
    if (callbacks) {
        if (callbacks->get<sol::optional<sol::function>>("primary"))
            element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)] = [=](bool down, glm::ivec2 pos) {
                callbacks->get<sol::function>("primary")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };

        if (callbacks->get<sol::optional<sol::function>>("secondary"))
            element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)] = [=](bool down, glm::ivec2 pos) {
                callbacks->get<sol::function>("secondary")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };

        if (callbacks->get<sol::optional<sol::function>>("hover"))
            element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)] = [=](bool down, glm::ivec2 pos) {
                callbacks->get<sol::function>("hover")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };
    }


    auto children = menu.get<sol::optional<sol::table>>("children");
    if (children) for (auto& pair : *children) element.children.push_back(rDeserialize(state, pair.second.as<sol::table>()));

    return element;
}

void GuiBuilder::build(glm::ivec2 winBounds) {
    clear(false);
    if (serialized.type != "")
        rCreate(serialized, root, winBounds);
}

void GuiBuilder::clear(bool deleteRoot) {
    rClearCallbacks(root);
    root->empty();
    if (deleteRoot) serialized = {"", ""};
}

void GuiBuilder::rCreate(const SerialGui::Element& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds) {
    auto component = createComponent(element, bounds);
    if (!component) throw std::runtime_error("GuiBuilder failed to create component: " + element.key);
    parent->add(component);

    for (auto& child : element.children) rCreate(child, component, component->getScale());
}

void GuiBuilder::rClearCallbacks(std::shared_ptr<GuiComponent> component) {
    component->setCallback(GuiComponent::CallbackType::PRIMARY, nullptr);
    component->setCallback(GuiComponent::CallbackType::SECONDARY, nullptr);
    component->setCallback(GuiComponent::CallbackType::HOVER, nullptr);

    for (auto& child : component->getChildren()) {
        rClearCallbacks(child);
    }
}

std::shared_ptr<GuiComponent> GuiBuilder::createComponent(const SerialGui::Element& elem, glm::ivec2 bounds) {
    std::shared_ptr<GuiComponent> c = nullptr;

    switch (Util::hash(elem.type.c_str())) {
        default: break;
        case Util::hash("body"): {
            auto body = GuiRect::fromSerialized(elem, textures, bounds);
            body->setScale(bounds);
            c = body;
            break;
        }
        case Util::hash("rect"):
            c = GuiRect::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("button"):
            c = GuiImageButton::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("text"):
            c = GuiText::fromSerialized(elem, textures, bounds);
            break;
        case Util::hash("model"):
            c = GuiModel::fromSerialized(elem, textures, models, bounds);
            break;
    }

    if (!c) return nullptr;

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)])
        c->setCallback(GuiComponent::CallbackType::PRIMARY, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)]);

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)])
        c->setCallback(GuiComponent::CallbackType::SECONDARY, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)]);

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)])
        c->setCallback(GuiComponent::CallbackType::HOVER, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)]);

    return c;
}

GuiBuilder::~GuiBuilder() {
    clear();
}
