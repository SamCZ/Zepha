//
// Created by aurailus on 08/01/19.
//

#pragma once

#include "../../game/ClientState.h"
#include "../../game/graph/scene/Scene.h"
#include "../hud/components/basic/GUIText.h"
#include "../hud/components/basic/GUIContainer.h"

class MainMenuScene : public Scene {
public:
    explicit MainMenuScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override {};

private:
    const int GS = 4;
    GUIContainer components;

    bool showingSubgame = true;
    std::shared_ptr<GUIRect> subgame;
};

