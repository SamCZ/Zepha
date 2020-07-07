//
// Created by aurailus on 2019-12-28.
//

#pragma once

#include "../graph/scene/Scene.h"

#include "../hud/components/basic/GuiContainer.h"

class LuaErrorScene : public Scene {
public:
    LuaErrorScene(ClientState& state, const std::string& err);

    void update() override;
    void draw() override;
    void cleanup() override;

private:
    GuiContainer components;
    const std::string err;
};
