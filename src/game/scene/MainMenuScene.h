//
// Created by aurailus on 08/01/19.
//

#pragma once

#include <json/json.hpp>

#include "../../game/graph/scene/Scene.h"

#include "menu/MenuSandbox.h"

class ClientState;

using nlohmann::json;

class MainMenuScene : public Scene {
public:
    explicit MainMenuScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override;
private:
    void positionElements();
    void findSubgames();

    const float GS = 3;

    glm::ivec2 win {};
    glm::ivec2 sandboxArea {};

    GuiContainer components;
    std::shared_ptr<GuiContainer> branding = nullptr;
    std::shared_ptr<GuiContainer> navigationBar = nullptr;
    std::shared_ptr<GuiContainer> menuContainer = std::make_shared<GuiContainer>("__menu");

    MenuSandbox sandbox;

    std::vector<Subgame> subgames;
    Subgame* selectedSubgame = nullptr;
};

