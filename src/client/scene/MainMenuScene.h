//
// Created by aurailus on 08/01/19.
//

#pragma once

#include "Scene.h"

#include "client/menu/SubgameDef.h"
#include "client/menu/MenuSandbox.h"

class Client;

class MainMenuScene : public Scene {
public:
    explicit MainMenuScene(Client& client);

    void update() override;
    void draw() override;

    void cleanup() override;
private:
    void positionElements();
    void findSubgames();

    static constexpr float GS = 3;

    glm::ivec2 win {};
    glm::ivec2 sandboxArea {};

    std::unique_ptr<GuiContainer> components;
    std::shared_ptr<GuiContainer> branding;
    std::shared_ptr<GuiContainer> navigationBar;
    std::shared_ptr<GuiContainer> menuContainer;

    MenuSandbox sandbox;

    std::vector<SubgameDef> subgames;
    SubgameDef* selectedSubgame = nullptr;
};

