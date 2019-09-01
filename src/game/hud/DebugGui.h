//
// Created by aurailus on 27/12/18.
//

#pragma once

#include <sstream>
#include "../graph/window/Window.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../../util/TransPos.h"
#include "../../util/Util.h"
#include "../../util/Ray.h"
#include "components/compound/GUILabelledGraph.h"
#include "components/basic/GUIRect.h"
#include "components/basic/GUIGraph.h"
#include "components/basic/GUIText.h"
#include "components/basic/GUIContainer.h"

class DebugGui : public GUIContainer {
public:
    DebugGui(glm::vec2 bufferSize, TextureAtlas& atlas);

    void bufferResized(glm::vec2 bufferSize);
    void changeVisibilityState(int state);
    void positionElements(glm::vec2 bufferSize);

    void update(Player& player, LocalWorld& world, LocalDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
private:
    int displayMode;
};
