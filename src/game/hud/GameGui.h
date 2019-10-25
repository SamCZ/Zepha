//
// Created by aurailus on 05/02/19.
//

#pragma once

#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../entity/Entity.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, TextureAtlas& atlas);
    void bufferResized(glm::vec2 bufferSize);

    void setVisible(bool visible) override;

    void setMenu(const std::string& state, std::shared_ptr<GUIComponent> component);
    void closeMenu();
    const std::string& getMenuState();

    std::string menuState = "";
};

