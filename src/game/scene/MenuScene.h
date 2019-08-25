//
// Created by aurailus on 08/01/19.
//

#pragma once

#include "../../game/ClientState.h"
#include "../../game/graph/scene/Scene.h"
#include "../entity/hud/components/basic/GUIText.h"
#include "../entity/hud/components/basic/GUIContainer.h"

class MenuScene : public Scene {
public:
    explicit MenuScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override {};

private:
    GUIContainer components;
};

