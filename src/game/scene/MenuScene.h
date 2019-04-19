//
// Created by aurailus on 08/01/19.
//

#ifndef ZEUS_MENUSCENE_H
#define ZEUS_MENUSCENE_H


#include "../../game/ClientState.h"
#include "../../game/graph/scene/Scene.h"
#include "../../game/entity/hud/HudText.h"

class MenuScene : public Scene {
public:
    explicit MenuScene(ClientState* state);

    void update() override;

    void draw() override;

    void cleanup() override;

private:
    Texture* fontTexture;
    std::vector<Entity*> entities;
//    DebugGui gui;
};


#endif //ZEUS_MENUSCENE_H
