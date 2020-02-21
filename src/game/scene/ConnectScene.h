//
// Created by aurailus on 11/07/19.
//

#pragma once

#include <utility>
#include "net/ServerConnection.h"
#include "../graph/scene/Scene.h"
#include "../hud/components/basic/GuiText.h"
#include "../hud/components/basic/GuiContainer.h"
#include "../hud/components/basic/GuiRect.h"
#include "../../lua/LuaModFile.h"
#include "../../lua/LuaMod.h"
#include "../../server/asset/AssetType.h"
#include "../../util/net/Deserializer.h"

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
