//
// Created by aurailus on 27/12/18.
//

#pragma once

#include <sstream>
#include "../graph/window/Window.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../../util/Space.h"
#include "../../util/Util.h"
#include "../../util/Ray.h"
#include "components/compound/GuiLabelledGraph.h"
#include "components/basic/GuiRect.h"
#include "components/basic/GuiGraph.h"
#include "components/basic/GuiText.h"
#include "components/basic/GuiContainer.h"
#include "../scene/world/InventoryList.h"

class DebugGui : public GuiContainer {
public:
    DebugGui(glm::vec2 bufferSize, ClientGame& atlas);

    void bufferResized(glm::vec2 bufferSize);
    void changeVisibilityState(int state);
    void positionElements(glm::vec2 bufferSize);

    void update(Player& player, LocalWorld& world, ClientGame& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
private:
    int displayMode;
};
