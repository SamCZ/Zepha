//
// Created by aurailus on 2019-12-28.
//

#pragma once

#include <string>
#include "../graph/scene/Scene.h"
#include "../hud/components/basic/GUIContainer.h"

class LuaErrorScene : public Scene {
public:
    LuaErrorScene(ClientState& state, const std::string& err);

    void update() override;
    void draw() override;
    void cleanup() override;

private:
    GUIContainer components;
    const std::string err;
};
