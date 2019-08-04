//
// Created by aurailus on 05/02/19.
//

#ifndef ZEUS_GAMEGUI_H
#define ZEUS_GAMEGUI_H

#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../ClientState.h"
#include "../Entity.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, TextureAtlas& atlas);
    void bufferResized(glm::vec2 bufferSize);

    void setVisible(bool visible) override;
    void setInvOpen(bool open);
    bool isInvOpen();

    bool invOpen;
};


#endif //ZEUS_GAMEGUI_H
