//
// Created by aurailus on 11/07/19.
//

#include "ConnectScene.h"

ConnectScene::ConnectScene(ClientState &state, Address addr) : Scene(state) {
    ServerConnection connection(std::move(addr), state.defs);
//    connection.
}
