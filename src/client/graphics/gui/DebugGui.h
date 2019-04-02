//
// Created by aurailus on 27/12/18.
//

#ifndef ZEUS_DEBUGGUI_H
#define ZEUS_DEBUGGUI_H

#include "../../engine/graphics/Histogram.h"
#include "../../engine/graphics/HudText.h"
#include "../../engine/Window.h"
#include "../../engine/Ray.h"
#include "../../engine/graphics/RectEntity.h"

#include <sstream>

class DebugGui {
public:
    explicit DebugGui(glm::vec2 bufferSize);

    void bufferResized(glm::vec2 bufferSize);

    void pushGuiObjects(std::vector<Entity*> &list);
    void update(Player* player, LocalWorld* world, Window* window, BlockAtlas* atlas, double fps, int chunks, int drawCalls, int ssGen, int ssPack);
    void positionElements(glm::vec2 bufferSize);

    ~DebugGui();

    void changeVisibilityState(int state);
private:
    int videoMemAvail, videoMemTotal;
    int displayMode;

    Texture *fontTexture;
    Texture *colorHistTexture, *whiteHistTexture, *transWhiteHistTexture;

    HudText    *dataText, *crosshairText;
    RectEntity *dataBG,   *crosshairBG;

    HudText    *chunkText, *meshText, *fpsText, *drawsText, *vramText, *ssGenText, *ssPackText;
    Histogram  *chunkHist, *meshHist, *fpsHist, *drawsHist, *tMeshHist, *vramHist, *ssGenHist, *ssPackHist;
    RectEntity *chunkBack, *meshBack, *fpsBack, *drawsBack, *vramBack, *ssGenBack, *ssPackBack;

    std::vector<Entity*> ownedElements;

    bool visible;
};


#endif //ZEUS_DEBUGGUI_H
