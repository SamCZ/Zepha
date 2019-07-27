//
// Created by aurailus on 11/07/19.
//

#ifndef ZEUS_CONNECTSCENE_H
#define ZEUS_CONNECTSCENE_H

#include <utility>
#include "../graph/scene/Scene.h"
#include "../entity/hud/components/TextEntity.h"
#include "net/ServerConnection.h"

class ConnectScene : public Scene {
public:
    enum class State {
        CONNECTING,
        FAILED_CONNECT,
        IDENTIFIER_LIST,
    };

    ConnectScene(ClientState& state, Address addr);

    void update() override;

    void handleConnecting();

    void draw() override;

    void cleanup() override {};
private:
    State connectState = State::CONNECTING;
    ServerConnection& connection;

    Texture fontTexture;
    TextEntity statusText;

    double dotsTime = 0;
};

#endif //ZEUS_CONNECTSCENE_H
