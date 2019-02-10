//
// Created by aurailus on 27/12/18.
//

#ifndef ZEUS_DEBUGGUI_H
#define ZEUS_DEBUGGUI_H

#include "../../engine/graphics/Histogram.h"
#include "../../engine/graphics/HudText.h"
#include "../../engine/Window.h"
#include "../../engine/Ray.h"

#include <sstream>

class DebugGui {
public:
    DebugGui();

    void pushGuiObjects(std::vector<Entity*> &list);
    void update(Player* player, World* world, Window* window, BlockAtlas* atlas, double fps);

    ~DebugGui();

private:
    Texture* fontTexture;
    Texture *fpsHist, *genHist;

    HudText*   fpsText;
    HudText*   alphaText;
    HudText*   playerText;
    Histogram *chunkUpdateHistogram, *meshUpdateHistogram, *fpsHistogram;
};


#endif //ZEUS_DEBUGGUI_H
