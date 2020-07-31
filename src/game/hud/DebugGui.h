//
// Created by aurailus on 27/12/18.
//

#pragma once

#include "components/basic/GuiContainer.h"

class LocalPlayer;
class LocalSubgame;
class LocalWorld;

class DebugGui : public GuiContainer {
public:
    DebugGui(glm::vec2 bufferSize, LocalSubgame& game, LocalWorld& world);

    void bufferResized(glm::vec2 bufferSize);
    void changeVisibilityState(int state);
    void positionElements(glm::vec2 bufferSize);

    void update(LocalPlayer& player, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
private:
    int displayMode;

    LocalWorld& world;
    LocalSubgame& game;
};
