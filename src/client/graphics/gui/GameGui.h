//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "../../engine/graphics/RectEntity.h"
#include "../../engine/Entity.h"
#include "../../ClientState.h"

class GameGui {
public:
    GameGui(glm::vec2 bufferSize);

    void bufferResized(glm::vec2 bufferSize);

    void pushGuiObjects(std::vector<Entity*> &list);

    ~GameGui();

    void setVisible(bool visible);
    bool isVisible();
private:
    Texture* crosshairTexture, *viginetteTexture;
    Entity* crosshair, *viginette;

    bool visible;
};


#endif //ZEUS_GAMEGUI_H
