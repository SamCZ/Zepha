//
// Created by aurailus on 11/07/19.
//

#include <thread>
#include <gzip/decompress.hpp>

#include "ConnectScene.h"

//#include "GameScene.h"
#include "client/Client.h"
#include "util/net/Packet.h"
#include "util/net/Address.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"
#include "client/gui/basic/GuiRect.h"
#include "client/gui/basic/GuiText.h"
#include "game/atlas/asset/AssetType.h"


/**
 * Initializes a connection to the remote address,
 * sets up the GUI, and attempts to download subgame assets.
 *
 * @param addr - The server address to init to.
 */

ConnectScene::ConnectScene(Client& client, Address addr) : Scene(client),
	connection(client.connection) {
	
	client.renderer.setClearColor(10, 10, 10);
	
//	Font f(client.game->textures, client.game->textures["font"]);
//
//	auto statusText = std::make_shared<GuiText>("statusText");
//	statusText->create({ 2, 2 }, {}, { 1, 1, 1, 1 }, {}, f);
//	statusText->setText("Connecting...");
//	statusText->setPos({ 32, 24 });
//	components.add(statusText);
//
//	auto loadBar = std::make_shared<GuiRect>("loadBar");
//	loadBar->create({ 1, 32 }, {}, { 0.17, 0.75, 0.93, 1 });
//	loadBar->setPos({ 0, client.renderer.window.getSize().y - 32 });
//	components.add(loadBar);
//
//	connection.attemptConnect(std::move(addr));
//
//	client.renderer.window.onResize([&](glm::ivec2 win) {
//		components.get<GuiRect>("loadBar")->setPos({ 0, win.y - 32 });
//	});
}

void ConnectScene::update() {
	client.game->textures.update();
	
	switch (connectState) {
	default:
		throw std::runtime_error("Invalid connection state.");
	
	case State::DONE:
	case State::FAILED_CONNECT:
		break;
	
	case State::CONNECTING:
		handleConnecting();
		break;
	
	case State::PROPERTIES: {
//		components.get<GuiRect>("loadBar")->setScale({ client.renderer.window.getSize().x * 0.1, 32 });
		
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::SERVER_INFO) {
//				auto statusText = components.get<GuiText>("statusText");
//				statusText->setText(statusText->getText() + "Received server properties.\n");
				
				const u32 seed = p.d.read<u32>();
//				std::cout << seed << std::endl;
				
				connectState = State::IDENTIFIER_LIST;
				Packet resp(Packet::Type::BLOCK_IDENTIFIER_LIST);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::IDENTIFIER_LIST: {
//		components.get<GuiRect>("loadBar")->setScale({ client.renderer.window.getSize().x * 0.2, 32 });
		
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::BLOCK_IDENTIFIER_LIST) {
//				auto statusText = components.get<GuiText>("statusText");
//				statusText->setText(statusText->getText() + "Received block index-identifier table.\n");
				
				client.game->getDefs().setIdentifiers(p.d.read<vec<string>>());
				
				Packet resp(Packet::Type::BIOME_IDENTIFIER_LIST);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
			else if (p.type == Packet::Type::BIOME_IDENTIFIER_LIST) {
//				auto statusText = components.get<GuiText>("statusText");
//				statusText->setText(
//					statusText->getText() + "Received biome index-identifier table.\nDownloading mods...\n");
				
				client.game->getBiomes().setIdentifiers(p.d.read<vec<string>>());
				
				connectState = State::MODS;
				Packet resp(Packet::Type::MODS);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::MODS: {
//		components.get<GuiRect>("loadBar")->setScale({ client.renderer.window.getSize().x * 0.4, 32 });
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
//			auto statusText = components.get<GuiText>("statusText");
			
			if (p.type == Packet::Type::MODS) {
				auto luaMod = LuaMod(p);
//				statusText->setText(statusText->getText() + "Received mod " + luaMod.config.name + ".\n");
				client.game->getParser().getHandler().addLuaMod(std::move(luaMod));
			}
			else if (p.type == Packet::Type::MOD_ORDER) {
				client.game->getParser().getHandler().setModsOrder(p.d.read<vec<string>>());
				
//				statusText->setText(statusText->getText() +
//					"Done downloading mods.\nReceived the mods order.\nReceiving media");
				
				connectState = State::MEDIA;
				Packet resp(Packet::Type::MEDIA);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::MEDIA: {
//		components.get<GuiRect>("loadBar")->setScale({ client.renderer.window.getSize().x * 0.6, 32 });
		
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
//			auto statusText = components.get<GuiText>("statusText");
			
			if (p.type == Packet::Type::MEDIA) {
				AssetType t = p.d.read<AssetType>();
				usize count = 0;
				
				while (t != AssetType::END) {
					string assetName = p.d.read<string>();
//					statusText->setText(statusText->getText() + ".");
					
					if (t == AssetType::TEXTURE) {
						u16 width = p.d.read<u16>();
						u16 height = p.d.read<u16>();
						
						string data = p.d.read<string>();
						string uncompressed = gzip::decompress(data.data(), data.length());
						
						client.game->textures.addImage(
							reinterpret_cast<unsigned char*>(const_cast<char*>(uncompressed.data())),
							assetName, true, width, height);
					}
					else if (t == AssetType::MODEL) {
						string format = p.d.read<string>();
						string data = p.d.read<string>();
						
						client.game->models.models.insert({ assetName, SerializedModel{ assetName, data, format }});
					}
					
					t = p.d.read<AssetType>();
					count++;
				}
				
			}
			else if (p.type == Packet::Type::MEDIA_DONE) {
//				statusText->setText(statusText->getText() + " Done.\nDone receiving media.\nJoining world...\n");
//				components.get<GuiRect>("loadBar")->setScale({ client.renderer.window.getSize().x, 32 });
				
				connectState = State::DONE;
//				client.scene.setScene(make_unique<GameScene>(client));
			}
		}
		break;
	}
	}
}

void ConnectScene::handleConnecting() {
//	auto statusText = components.get<GuiText>("statusText");
	
	switch (connection.getConnectionStatus()) {
	default:
		throw std::runtime_error("Uncaught connection error.");
	
	case ServerConnection::State::ENET_ERROR:
		throw std::runtime_error("Enet Initialization error.");
	
	case ServerConnection::State::FAILED_CONNECT:
		connectState = State::FAILED_CONNECT;
//		statusText->setText(statusText->getText() + "\nFailed to init :(\n");
		break;
	
	case ServerConnection::State::ATTEMPTING_CONNECT:
		connection.processConnecting();
		
		dotsTime += client.getDelta();
		if (dotsTime > 1) {
			dotsTime -= 1;
//			statusText->setText(statusText->getText() + ".");
		}
		
		break;
	
	case ServerConnection::State::CONNECTED: {
		connectState = State::PROPERTIES;
//		statusText->setText(statusText->getText() + " Connected!~\n");
		
		Packet resp(Packet::Type::SERVER_INFO);
		resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
		break;
	}
	}
}

void ConnectScene::draw() {
	Renderer& renderer = client.renderer;
	
	renderer.beginChunkDeferredCalls();
	renderer.endDeferredCalls();
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	
//	components.draw(renderer);
}