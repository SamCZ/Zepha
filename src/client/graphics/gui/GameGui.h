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
    explicit GameGui(ClientState* state);

    void pushGuiObjects(std::vector<Entity*> &list);

    ~GameGui();
private:
    Texture* crosshairTexture;
    Entity* crosshair;
};


#endif //ZEUS_GAMEGUI_H
