//
// Created by aurailus on 2019-12-28.
//

#pragma once

#include "Scene.h"

#include "client/gui/basic/GuiContainer.h"

class LuaErrorScene : public Scene {
public:
    LuaErrorScene(Client& client, const std::string& err);

    void update() override;
    void draw() override;
    void cleanup() override;

private:
    GuiContainer components;
    const std::string err;
};
