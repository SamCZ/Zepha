//
// Created by aurailus on 11/07/19.
//

#pragma once

#include "../graph/scene/Scene.h"
#include "../hud/components/basic/GuiContainer.h"

class ConnectScene : public Scene {
public:
    enum class State {
        CONNECTING,
        FAILED_CONNECT,
        IDENTIFIER_LIST,
        MODS,
        MEDIA,
        DONE
    };

    ConnectScene(ClientState& state, Address addr);

    void update() override;

    void handleConnecting();

    void draw() override;

    void cleanup() override;
private:
    State connectState = State::CONNECTING;
    ServerConnection& connection;

    GuiContainer components;

    double dotsTime = 0;
};
