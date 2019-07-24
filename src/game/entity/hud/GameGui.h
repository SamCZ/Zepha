//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "TextureRect.h"
#include "../Entity.h"
#include "../../ClientState.h"
#include "../../graph/drawable/DrawableGroup.h"

class GameGui : public DrawableGroup {
public:
    explicit GameGui(glm::vec2 bufferSize);

    void bufferResized(glm::vec2 bufferSize);

    void setVisible(bool visible) override;
private:
    Texture crosshairTexture;
    Texture viginetteTexture;

    TextureRect* crosshair;
    TextureRect* viginette;
};


#endif //ZEUS_GAMEGUI_H
