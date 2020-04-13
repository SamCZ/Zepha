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

void GuiBuilder::setGuiRoot(sol::state_view state, LocalGuiElement& menu) {
    keyInd = 0;
    serialized = recursivelyDeserialize(state, menu);
}

SerialGui::Element GuiBuilder::recursivelyDeserialize(sol::state_view state, LocalGuiElement& elem) {
    std::string key = (elem.key.size() ? elem.key : "__UNKEYED_COMPONENT_" + std::to_string(keyInd++));
    SerialGui::Element element(elem.type, key);

    for (const auto& trait : elem.traits) {
        auto key = trait.first;
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

    if (elem.callbacks.count("primary"))
        element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)] = [=](bool down, glm::ivec2 pos) {
            elem.callbacks.at("primary")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };

    if (elem.callbacks.count("secondary"))
        element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)] = [=](bool down, glm::ivec2 pos) {
            elem.callbacks.at("secondary")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };

    if (elem.callbacks.count("secondary"))
        element.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)] = [=](bool down, glm::ivec2 pos) {
            elem.callbacks.at("hover")(down, LuaParser::luaVec(state, {pos.x, pos.y, 0})); };

    for (auto& child : elem.children) element.children.push_back(recursivelyDeserialize(state, child));
    return element;
}

void GuiBuilder::build(glm::ivec2 winBounds) {
    clear(false);
    if (serialized.type != "")
        recursivelyCreate(serialized, root, winBounds);
}

void GuiBuilder::clear(bool deleteRoot) {
    recursivelyClearCallbacks(root);
    root->empty();
    if (deleteRoot) serialized = {"", ""};
}

void GuiBuilder::recursivelyCreate(const SerialGui::Element& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds) {
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

std::shared_ptr<GuiComponent> GuiBuilder::createComponent(const SerialGui::Element& elem, glm::ivec2 bounds) {
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
