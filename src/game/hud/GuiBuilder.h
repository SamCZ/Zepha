//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "SerialGui.h"
#include "components/GuiComponent.h"
#include "../../def/ClientGame.h"
#include "../../lua/api/class/LuaGuiElement.h"

class GuiContainer;

class GuiBuilder {
public:
    struct ComponentCallbacks { GuiComponent::callback left {}, right {}, hover {}; };

    GuiBuilder(TextureAtlas& textures, ModelStore& models, std::shared_ptr<GuiContainer> root);
    void setGuiRoot(sol::state_view state, LuaGuiElement& menu);
    void build(glm::ivec2 winBounds);
    void clear(bool deleteRoot = true);

    ~GuiBuilder();
protected:
    void recursivelyCreate(const LuaGuiElement& element, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds);
    static void recursivelyClearCallbacks(std::shared_ptr<GuiComponent> component);
    virtual std::shared_ptr<GuiComponent> createComponent(const LuaGuiElement& elem, glm::ivec2 bounds);

    TextureAtlas& textures;
    ModelStore& models;

    std::shared_ptr<GuiContainer> root = nullptr;

    LuaGuiElement* elements = nullptr;
    unsigned int keyInd = 0;
};
