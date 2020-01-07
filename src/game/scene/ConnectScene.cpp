//
// Created by aurailus on 11/07/19.
//

#include <gzip/decompress.hpp>
#include "ConnectScene.h"
#include "../../server/asset/AssetType.h"

ConnectScene::ConnectScene(ClientState &state, Address addr) : Scene(state),
    connection(state.connection) {

    state.renderer.setClearColor(10, 10, 10);

    Font f(state.defs.textures, state.defs.textures["font"]);

    auto statusText = std::make_shared<GUIText>("statusText");
    statusText->create({2, 2}, {}, {}, {1, 1, 1, 1}, f);
    statusText->setText("Connecting...");
    statusText->setPos({32, 24});
    components.add(statusText);

    auto loadBar = std::make_shared<GUIRect>("loadBar");
    loadBar->create({1, 32}, {}, {0.17, 0.75, 0.93, 1});
    loadBar->setPos({0, state.renderer.window.getSize().y - 32});
    components.add(loadBar);

    connection.attemptConnect(std::move(addr));

    state.renderer.window.addResizeCallback("scene", [&](glm::ivec2 win) {
        components.get<GUIRect>("loadBar")->setPos({0, win.y - 32});
    });
}

void ConnectScene::update() {
    state.defs.textures.update();

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
            components.get<GUIRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.2, 32});
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);

                if (p.type == PacketType::BLOCK_IDENTIFIER_LIST) {
                    auto statusText = components.get<GUIText>("statusText");
                    statusText->setText(statusText->getText() + "Received block index-identifier table.\n");

                    std::vector<std::string> indexIdentifierTable{};
                    indexIdentifierTable.reserve(static_cast<unsigned long>(OLDSerializer::decodeUInt(&p.data[0])));

                    unsigned int ind = 4;
                    while (true) {
                        auto len = OLDSerializer::decodeInt(&p.data[ind]);
                        indexIdentifierTable.emplace_back(&p.data[ind + 4], &p.data[ind + 4 + len]);
                        ind += 4 + len;
                        if (ind >= p.data.length() - 4) break;
                    }

                    state.defs.defs.setIdentifiers(indexIdentifierTable);

                    Packet resp(PacketType::BIOME_IDENTIFIER_LIST);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
                else if (p.type == PacketType::BIOME_IDENTIFIER_LIST) {
                    auto statusText = components.get<GUIText>("statusText");
                    statusText->setText(statusText->getText() + "Received biome index-identifier table.\nDownloading mods...\n");

                    std::vector<std::string> indexIdentifierTable{};
                    indexIdentifierTable.reserve(static_cast<unsigned long>(OLDSerializer::decodeUInt(&p.data[0])));

                    unsigned int ind = 4;
                    while (true) {
                        auto len = OLDSerializer::decodeInt(&p.data[ind]);
                        indexIdentifierTable.emplace_back(&p.data[ind + 4], &p.data[ind + 4 + len]);
                        ind += 4 + len;
                        if (ind >= p.data.length() - 4) break;
                    }

                    state.defs.biomes.setIdentifiers(indexIdentifierTable);

                    connectState = State::MODS;
                    Packet resp(PacketType::MODS);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::MODS: {
            components.get<GUIRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.4, 32});
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);
                auto statusText = components.get<GUIText>("statusText");

                if (p.type == PacketType::MODS) {
                    auto luaMod = LuaMod::fromPacket(p);
                    statusText->setText(statusText->getText() + "Received mod " + luaMod.config.name + ".\n");
                    state.defs.luaApi.mods.push_back(std::move(luaMod));
                }
                else if (p.type == PacketType::MOD_ORDER) {
                    std::string order = OLDSerializer::decodeString(&p.data[0]);

                    size_t pos = 0;
                    std::string token;
                    while ((pos = order.find(',')) != std::string::npos) {
                        token = order.substr(0, pos);
                        state.defs.luaApi.modsOrder.push_back(token);
                        order.erase(0, pos + 1);
                    }
                    state.defs.luaApi.modsOrder.push_back(order);

                    statusText->setText(statusText->getText() + "Done downloading mods.\nReceived the mods order.\nDownloading media...\n");

                    connectState = State::MEDIA;
                    Packet resp(PacketType::MEDIA);
                    resp.sendTo(connection.getPeer(), PacketChannel::CONNECT);
                }
            }
            break;
        }

        case State::MEDIA: {
            components.get<GUIRect>("loadBar")->setScale({state.renderer.window.getSize().x * 0.6, 32});
            ENetEvent e;
            if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
                Packet p(e.packet);
                auto statusText = components.get<GUIText>("statusText");

                if (p.type == PacketType::MEDIA) {
                    AssetType t = static_cast<AssetType>(OLDSerializer::decodeInt(&p.data[0]));
                    size_t offset = 4;
                    unsigned int count = 0;

                    while (t != AssetType::END) {
                        std::string assetName = OLDSerializer::decodeString(&p.data[offset]);
                        offset += assetName.length() + 4;

                        if (t == AssetType::TEXTURE) {
                            int width = OLDSerializer::decodeInt(&p.data[offset]);
                            offset += 4;
                            int height = OLDSerializer::decodeInt(&p.data[offset]);
                            offset += 4;

                            std::string data = OLDSerializer::decodeString(&p.data[offset]);
                            std::string uncompressed = gzip::decompress(data.data(), data.length());

                            state.defs.textures.addImage(
                                    reinterpret_cast<unsigned char *>(const_cast<char *>(uncompressed.data())),
                                    assetName, true, width, height);

                            offset += data.length() + 4;
                        }
                        else if (t == AssetType::MODEL) {
                            std::string format = OLDSerializer::decodeString(&p.data[offset]);
                            offset += format.length() + 4;
                            std::string data = OLDSerializer::decodeString(&p.data[offset]);
                            offset += data.length() + 4;

                            state.defs.models.models.insert({assetName, SerializedModel{assetName, data, format}});
                        }

                        count++;

                        t = static_cast<AssetType>(OLDSerializer::decodeInt(&p.data[offset]));
                        offset += 4;
                    }

                    statusText->setText(statusText->getText() + "Received " + to_string(count) + "x media files.\n");
                }
                else if (p.type == PacketType::MEDIA_DONE) {
                    components.get<GUIRect>("loadBar")->setScale({state.renderer.window.getSize().x, 32});
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
    Packet resp(PacketType::BLOCK_IDENTIFIER_LIST);
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
    renderer.enableTexture(&state.defs.textures.atlasTexture);

    components.draw(renderer);

    renderer.swapBuffers();
}

void ConnectScene::cleanup() {
    state.renderer.window.removeResizeCallback("scene");
}