//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "RectEntity.h"
#include "../Entity.h"
#include "../../ClientState.h"
#include "../../graph/drawable/DrawableGroup.h"

class GameGui : public DrawableGroup {
public:
    explicit GameGui(glm::vec2 bufferSize);

    void bufferResized(glm::vec2 bufferSize);

    ~GameGui() override;

    void setVisible(bool visible);
    bool isVisible();
private:
    Texture* crosshairTexture, *viginetteTexture;
    Entity* crosshair, *viginette;

    bool visible;
};


#endif //ZEUS_GAMEGUI_H
