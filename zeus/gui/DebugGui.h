//
// Created by aurailus on 27/12/18.
//

#ifndef ZEUS_DEBUGGUI_H
#define ZEUS_DEBUGGUI_H


#include "../engine/graphics/HudText.h"
#include "../game/world/World.h"
#include <sstream>

class DebugGui {
public:
    DebugGui();

    void pushGuiObjects(std::vector<Entity*> &list);
    void update(glm::vec3 pos, std::string block, double fps);

    ~DebugGui();

private:
    void fpsHistUpdate();

    Texture* fontTexture;
    Texture* histogramTexture;

    HudText* fpsText;
    HudText* alphaText;
    HudText* blockText;
    Entity* fpsHistogram;

    const int FPS_HISTOGRAM_SIZE = 120;
    std::vector<double> fpsHistory;
};


#endif //ZEUS_DEBUGGUI_H
