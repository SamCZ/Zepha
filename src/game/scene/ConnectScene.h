//
// Created by aurailus on 11/07/19.
//

#pragma once

#include <utility>
#include "net/ServerConnection.h"
#include "../graph/scene/Scene.h"
#include "../entity/hud/components/basic/GUIText.h"
#include "../entity/hud/components/basic/GUIContainer.h"
#include "../entity/hud/components/basic/GUIRect.h"
#include "../../api/LuaModFile.h"
#include "../../api/LuaMod.h"

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

    void cleanup() override {};
private:
    State connectState = State::CONNECTING;
    ServerConnection& connection;

    GUIContainer components;

    double dotsTime = 0;
};
