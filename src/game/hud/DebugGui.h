//
// Created by aurailus on 27/12/18.
//

#pragma once

#include "components/basic/GuiContainer.h"

class Player;
class LocalSubgame;
class LocalWorld;

class DebugGui : public GuiContainer {
public:
    DebugGui(glm::vec2 bufferSize, LocalSubgame& atlas);

    void bufferResized(glm::vec2 bufferSize);
    void changeVisibilityState(int state);
    void positionElements(glm::vec2 bufferSize);

    void update(Player& player, LocalWorld& world, LocalSubgame& game, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
private:
    int displayMode;
};
