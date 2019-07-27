//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "components/basic/GUIRect.h"
#include "../Entity.h"
#include "../../ClientState.h"
#include "../../graph/drawable/DrawableGroup.h"

class GameGui : public Drawable {
public:
    explicit GameGui(glm::vec2 bufferSize, TextureAtlas& atlas);

    void draw(Renderer& renderer) override;

    void bufferResized(glm::vec2 bufferSize);
    void setVisible(bool visible) override;
private:
    std::shared_ptr<AtlasRef> crosshairRef;
    std::shared_ptr<AtlasRef> viginetteRef;

    GUIRect crosshair;
    GUIRect viginette;
};


#endif //ZEUS_GAMEGUI_H
