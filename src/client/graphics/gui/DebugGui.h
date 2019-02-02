//
// Created by aurailus on 27/12/18.
//

#ifndef ZEUS_DEBUGGUI_H
#define ZEUS_DEBUGGUI_H

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
    void fpsHistUpdate();

    Texture* fontTexture;
    Texture* histogramTexture;

    HudText* fpsText;
    HudText* alphaText;
    HudText* playerText;
    Entity* fpsHistogram;

    const int FPS_HISTOGRAM_SIZE = 120;
    std::vector<double> fpsHistory;
};


#endif //ZEUS_DEBUGGUI_H
