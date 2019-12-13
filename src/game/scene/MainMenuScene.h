//
// Created by aurailus on 08/01/19.
//

#pragma once

#include <json/json.hpp>
#include "../../game/ClientState.h"
#include "../../game/graph/scene/Scene.h"
#include "../hud/components/basic/GUIText.h"
#include "../hud/components/basic/GUIContainer.h"
#include "../hud/components/compound/GUIImageButton.h"
#include "menu/Subgame.h"
#include "menu/MenuSandbox.h"

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

    GUIContainer components;
    std::shared_ptr<GUIContainer> branding = nullptr;
    std::shared_ptr<GUIContainer> navigationBar = nullptr;
    std::shared_ptr<GUIContainer> sandboxContainer = std::make_shared<GUIContainer>("_sandbox");

    MenuSandbox sandbox;

    std::vector<Subgame> subgames;
    Subgame* selectedSubgame = nullptr;
};

