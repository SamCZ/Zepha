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

using nlohmann::json;

class MainMenuScene : public Scene {
public:
    explicit MainMenuScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override {};

private:
    void positionElements(glm::ivec2 win);
    void findSubgames();

    std::vector<Subgame> subgames;

    const float GS = 4;
    GUIContainer components;

    bool showingSubgame = true;
    std::shared_ptr<GUIRect> subgame;

    std::shared_ptr<GUIContainer> branding = nullptr;
    std::shared_ptr<GUIContainer> navigationBar = nullptr;
};

