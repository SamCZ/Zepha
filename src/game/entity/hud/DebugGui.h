//
// Created by aurailus on 27/12/18.
//

#ifndef ZEUS_DEBUGGUI_H
#define ZEUS_DEBUGGUI_H

#include <sstream>
#include "../../graph/window/Window.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../../util/TransPos.h"
#include "../../../util/Util.h"
#include "../../../util/Ray.h"
#include "StatGraph.h"
#include "components/basic/GUIRect.h"
#include "components/GraphEntity.h"
#include "components/basic/GUIText.h"
#include "components/basic/GUIContainer.h"

class DebugGui : public Drawable {
public:
    DebugGui(glm::vec2 bufferSize, TextureAtlas& atlas);

    void bufferResized(glm::vec2 bufferSize);

    void update(Player& player, LocalWorld& world, LocalDefs& defs, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
    void draw(Renderer &renderer) override;

    void changeVisibilityState(int state);
private:
    void positionElements(glm::vec2 bufferSize);

    GUIContainer components;

    int displayMode;

//    StatGraph chunkUpdatesGraph;
//    StatGraph meshUpdatesGraph;
//    StatGraph fpsGraph;
//    StatGraph drawCallsGraph;
//    StatGraph vRamGraph;
//    StatGraph serverGenGraph;
//    StatGraph serverPacketGraph;
};

#endif //ZEUS_DEBUGGUI_H
