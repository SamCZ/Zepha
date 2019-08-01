//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../Entity.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../ClientState.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, TextureAtlas& atlas);
    void bufferResized(glm::vec2 bufferSize);

    void setInventoryVisible(bool visible);
    bool isInventoryVisible();
};


#endif //ZEUS_GAMEGUI_H
