//
// Created by aurailus on 08/01/19.
//

#ifndef ZEUS_MENUSCENE_H
#define ZEUS_MENUSCENE_H


#include "../client/ClientState.h"
#include "../client/Scene.h"

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
