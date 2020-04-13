//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "SerialGui.h"
#include "components/GuiComponent.h"
#include "../../def/ClientGame.h"
#include "../../lua/api/class/LocalGuiElement.h"

class GuiContainer;

class GuiBuilder {
public:
    struct ComponentCallbacks { GuiComponent::callback left {}, right {}, hover {}; };

    GuiBuilder(TextureAtlas& textures, ModelStore& models, std::shared_ptr<GuiContainer> root);
    void setGuiRoot(sol::state_view state, LocalGuiElement& menu);
    void build(glm::ivec2 winBounds);
    void clear(bool deleteRoot = true);

    ~GuiBuilder();
protected:
    SerialGui::Element recursivelyDeserialize(sol::state_view state, LocalGuiElement& elem);
    void recursivelyCreate(const SerialGui::Element& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds);
    static void recursivelyClearCallbacks(std::shared_ptr<GuiComponent> component);
    virtual std::shared_ptr<GuiComponent> createComponent(const SerialGui::Element& elem, glm::ivec2 bounds);

    TextureAtlas& textures;
    ModelStore& models;

    std::shared_ptr<GuiContainer> root = nullptr;

    SerialGui::Element serialized = {"", ""};
    unsigned int keyInd = 0;
};
