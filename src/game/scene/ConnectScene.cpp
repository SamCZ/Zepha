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
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);

                auto statusText = components.get<GUIText>("statusText");
                statusText->setText(statusText->getText() + "Received block index-identifier table.\nDownloading mods...\n");

                std::vector<std::string> indexIdentifierTable {};
                indexIdentifierTable.reserve(static_cast<unsigned long>(Serializer::decodeInt(&p.data[0])));

                unsigned int ind = 4;
                while (true) {
                    auto len = Serializer::decodeInt(&p.data[ind]);
                    indexIdentifierTable.emplace_back(&p.data[ind + 4], &p.data[ind + 4 + len]);
                    ind += 4 + len;
                    if (ind >= p.data.length() - 4) break;
                }

                state.defs.defs().setIdentifiers(indexIdentifierTable);

                connectState = State::MODS;
                Packet resp(PacketType::MODS);
                resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
            }
            break;
        }

        case State::MODS: {
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);
                auto statusText = components.get<GUIText>("statusText");

                if (p.type == PacketType::MODS) {
                    auto luaMod = LuaMod::fromPacket(p);
                    statusText->setText(statusText->getText() + "Received mod " + luaMod.config.name + ".\n");
                    state.defs.lua().mods.push_back(std::move(luaMod));
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

                    statusText->setText(statusText->getText() + "Done downloading mods.\nReceived the mods order.\nDownloading media...\n");

                    connectState = State::MEDIA;
                    Packet resp(PacketType::MEDIA);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::MEDIA: {
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);
                auto statusText = components.get<GUIText>("statusText");

                if (p.type == PacketType::MEDIA) {
                    std::string texName = Serializer::decodeString(&p.data[0]);
                    size_t offset = texName.length() + 4;
                    unsigned int count = 0;

                    while (texName != "end") {
                        int width = Serializer::decodeInt(&p.data[offset]);
                        offset += 4;
                        int height = Serializer::decodeInt(&p.data[offset]);
                        offset += 4;

                        std::string data = Serializer::decodeString(&p.data[offset]);
                        std::string uncompressed = gzip::decompress(data.data(), data.length());

                        state.defs.textures().addImage(reinterpret_cast<unsigned char*>(const_cast<char*>(uncompressed.data())), texName, true, width, height);

                        offset += data.length() + 4;
                        count++;

                        texName = Serializer::decodeString(&p.data[offset]);
                        offset += texName.length() + 4;
                    }

                    statusText->setText(statusText->getText() + "Received " + to_string(count) + "x media files.\n");

//                    state.defs.lua().mods.push_back(std::move(luaMod));'
                }
                else if (p.type == PacketType::MEDIA_DONE) {
//                    std::string order = Serializer::decodeString(&p.data[0]);
//
//                    size_t pos = 0;
//                    std::string token;
//                    while ((pos = order.find(',')) != std::string::npos) {
//                        token = order.substr(0, pos);
//                        state.defs.lua().modsOrder.push_back(token);
//                        order.erase(0, pos + 1);
//                    }
//                    state.defs.lua().modsOrder.push_back(order);


                    statusText->setText(statusText->getText() + "Done downloading media.\nJoining world...\n");

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