//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "SerialGui.h"
#include "../../def/ClientGame.h"
#include "components/GuiComponent.h"

class GuiContainer;

class GuiBuilder {
public:
    struct ComponentCallbacks { GuiComponent::callback left {}, right {}, hover {}; };

    GuiBuilder(ClientGame& defs, std::shared_ptr<GuiContainer> root);
    void setGui(const std::string& menu, const std::map<std::string, ComponentCallbacks>& callbacks = {});
    void clear(bool clrCallbacks = true);
    void build(glm::ivec2 winBounds);

    ~GuiBuilder();

protected:
    void deserialize(const std::string& menu);
    void recursivelyCreate(std::vector<SerialGui::Elem> components, std::shared_ptr<GuiComponent> parent, glm::ivec2 bounds);
    virtual std::shared_ptr<GuiComponent> createComponent(SerialGui::Elem& component, glm::ivec2 bounds);

    std::map<std::string, ComponentCallbacks> callbacks;

    ClientGame& game;
    std::shared_ptr<GuiContainer> root;
    std::vector<SerialGui::Elem> components {};
};
