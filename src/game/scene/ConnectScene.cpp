//
// Created by aurailus on 11/07/19.
//

#include <gzip/decompress.hpp>
#include "ConnectScene.h"

ConnectScene::ConnectScene(ClientState &state, Address addr) : Scene(state),
    connection(state.connection) {

    state.renderer.setClearColor(10, 10, 10);

    auto statusText = std::make_shared<GUIText>("statusText");
    statusText->create({2, 2}, {}, {}, {}, state.defs.textures().getTextureRef("font"));
    statusText->setText("Connecting...");
    statusText->setPos({32, 24});
    components.add(statusText);

    connection.attemptConnect(std::move(addr));
}

void ConnectScene::update() {
    switch (connectState) {
        default:
            std::cout << Log::err << "Invalid connectState" << Log::endl;
            exit(1);

        case State::DONE:
        case State::FAILED_CONNECT:
            break;

        case State::CONNECTING:
            handleConnecting();
            break;

        case State::IDENTIFIER_LIST: {
            ENetEvent e;
            while (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);

                auto statusText = components.get<GUIText>("statusText");
                statusText->setText(statusText->getText() + "Received block index-identifier table.\n");

                std::vector<std::string> indexIdentifierTable {};
                indexIdentifierTable.reserve(static_cast<unsigned long>(Serializer::decodeInt(&p.data[0])));

                unsigned int ind = 4;
                while (true) {
                    auto len = Serializer::decodeInt(&p.data[ind]);
                    indexIdentifierTable.emplace_back(&p.data[ind + 4], &p.data[ind + 4 + len]);
                    ind += 4 + len;
                    if (ind >= p.data.length() - 4) break;
                }

                state.defs.blocks().setIdentifiers(indexIdentifierTable);

                connectState = State::MODS;
                Packet resp(PacketType::MODS);
                resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
            }
            break;
        }

        case State::MODS: {
            ENetEvent e;
            while (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);
                auto statusText = components.get<GUIText>("statusText");

                if (p.type == PacketType::MODS) {
                    std::string mod = gzip::decompress(&p.data[4], p.data.size());

                    LuaMod luaMod {};
                    luaMod.serialized = &p.data[4];

                    unsigned int offset = 0;

                    std::string name = Serializer::decodeString(&mod[0]);
                    offset += 4 + name.length();

                    std::string description = Serializer::decodeString(&mod[offset]);
                    offset += 4 + description.length();

                    std::string version = Serializer::decodeString(&mod[offset]);
                    offset += 4 + version.length();

                    std::string dependsStr = Serializer::decodeString(&mod[offset]);
                    offset += 4 + dependsStr.length();

                    std::vector<std::string> depends;
                    size_t pos = 0;
                    std::string token;
                    while ((pos = dependsStr.find(',')) != std::string::npos) {
                        token = dependsStr.substr(0, pos);
                        depends.push_back(token);
                        dependsStr.erase(0, pos + 1);
                    }
                    depends.push_back(dependsStr);

                    luaMod.config = {name, description, version, depends};

                    while (offset < mod.length()) {
                        std::string path = Serializer::decodeString(&mod[offset]);
                        offset += 4 + path.length();
                        std::string file = Serializer::decodeString(&mod[offset]);
                        offset += 4 + file.length();

                        LuaModFile modFile {path, file};
                        luaMod.files.push_back(modFile);
                    }

                    state.defs.lua().mods.push_back(std::move(luaMod));
                    statusText->setText(statusText->getText() + "Received " + name + ".\n");
                }
                else if (p.type == PacketType::MOD_ORDER) {
                    std::string order = Serializer::decodeString(&p.data[0]);

                    size_t pos = 0;
                    std::string token;
                    while ((pos = order.find(',')) != std::string::npos) {
                        token = order.substr(0, pos);
                        state.defs.lua().modsOrder.push_back(token);
                        order.erase(0, pos + 1);
                    }
                    state.defs.lua().modsOrder.push_back(order);


                    statusText->setText(statusText->getText() + "Received the mod order.\n");
                    statusText->setText(statusText->getText() + "Joining World...\n");

                    connectState = State::DONE;
                    state.desiredState = "game";
                }
            }
            break;
        }
    }
}

void ConnectScene::handleConnecting() {
    Packet resp(PacketType::IDENTIFIER_LIST);
    auto statusText = components.get<GUIText>("statusText");

    switch (connection.getConnectionStatus()) {
        default:
            std::cout << Log::err << "Undefined connection error. Exiting." << Log::endl;

        case ServerConnection::State::ENET_ERROR:
            exit(1);
            break;

        case ServerConnection::State::FAILED_CONNECT:
            connectState = State::FAILED_CONNECT;
            statusText->setText(statusText->getText() + "\nFailed to connect :(\n");
            break;

        case ServerConnection::State::ATTEMPTING_CONNECT:
            connection.processConnecting();

            dotsTime += state.deltaTime;
            if (dotsTime > 1) {
                dotsTime -= 1;
                statusText->setText(statusText->getText() + ".");
            }

            break;

        case ServerConnection::State::CONNECTED:
            connectState = State::IDENTIFIER_LIST;
            statusText->setText(statusText->getText() + " Connected!~\n");

            resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);

            break;
    }
}

void ConnectScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&state.defs.textures().getAtlasTexture());

    components.draw(renderer);

    renderer.swapBuffers();
}