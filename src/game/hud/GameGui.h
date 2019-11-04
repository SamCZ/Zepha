//
// Created by aurailus on 05/02/19.
//

#pragma once

#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../entity/Entity.h"
#include "SerializedGuiElem.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, TextureAtlas& atlas);
    void bufferResized(glm::vec2 bufferSize);

    void setVisible(bool visible) override;

    void setMenu(const std::string& menu);
    void closeMenu();
    const std::string& getMenuState();

private:
    void recursivelyCreate(std::vector<SerializedGuiElem> components, std::shared_ptr<GUIComponent> parent);
    std::shared_ptr<GUIComponent> createComponent(SerializedGuiElem& component);
    std::vector<SerializedGuiElem> components {};

    float deserializeNumber(const std::string& input, bool height);
    glm::vec2 deserialize2DVector(const std::string& input);
    glm::vec4 deserialize4DVector(const std::string& input);

    glm::vec2 bufferSize {};
    TextureAtlas& atlas;
    std::string menuState = "";
};

