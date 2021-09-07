#include <thread>
#include <gzip/decompress.hpp>

#include "ConnectScene.h"

#include "GameScene.h"
#include "client/Client.h"
#include "util/net/Packet.h"
#include "util/net/Address.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"
#include "client/gui/TextElement.h"
#include "game/atlas/asset/AssetType.h"

ConnectScene::ConnectScene(Client& client, Address addr) : Scene(client),
	root(client.renderer.window, client.game->textures),
	connection(client.connection) {
	
	client.renderer.setClearColor(10, 10, 10);
	
	using Expr = Gui::Expression;
	status = root.body->append<Gui::TextElement>({{
		{ Gui::Prop::TEXT_SIZE, Expr("2px") },
		{ Gui::Prop::SIZE, array<Expr, 2> { Expr("100dp"), Expr("-1") } },
		{ Gui::Prop::CONTENT, string("`c1Connecting to `b`c0" + addr.toString() + "`r`c1...") },
		{ Gui::Prop::MARGIN, array<Expr, 4> { Expr("4dp"), Expr("4dp"), Expr("4dp"), Expr("4dp") } }
	}});
	
	connection.attemptConnect(std::move(addr));
}

void ConnectScene::update() {
	client.game->textures.update();
	root.update();
	
	switch (state) {
	case State::DONE:
	case State::FAILED_CONNECT:
		break;
	
	case State::CONNECTING:
		handleConnecting();
		break;
	
	case State::PROPERTIES: {
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::SERVER_INFO) {
				status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "")
					+ "Received server properties.\n");
				
				const u32 seed = p.d.read<u32>();
				std::cout << seed << std::endl;
				
				state = State::IDENTIFIER_LIST;
				Packet resp(Packet::Type::BLOCK_IDENTIFIER_LIST);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::IDENTIFIER_LIST: {
		
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::BLOCK_IDENTIFIER_LIST) {
				client.game->getDefs().setIdentifiers(p.d.read<vec<string>>());
				Packet resp(Packet::Type::BIOME_IDENTIFIER_LIST);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
			else if (p.type == Packet::Type::BIOME_IDENTIFIER_LIST) {
				status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "")
					+ "Received block & biome index-identifier table.\nDownloading mods... ");
				
				client.game->getBiomes().setIdentifiers(p.d.read<vec<string>>());
				state = State::MODS;
				Packet resp(Packet::Type::MODS);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::MODS: {
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::MODS) {
				auto mod = Mod(p);
				status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "") +
	                (modsFound == 0 ? "" : ", ") +
	                ((modsFound) % 8 == 0 && modsFound != 0 ? "\n" : "") +
	                "`c0`u" + mod.name + "`r`c1");
				modsFound++;
				client.game->getParser().addMod(std::move(mod));
			}
			else if (p.type == Packet::Type::MOD_ORDER) {
				client.game->getParser().setModLoadOrder(p.d.read<vec<string>>());
				
				status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "")
					+ ".\n`c7Done`c1 downloading mods. Received the mods order.\nReceiving media");
				
				state = State::MEDIA;
				Packet resp(Packet::Type::MEDIA);
				resp.sendTo(connection.getPeer(), Packet::Channel::CONNECT);
			}
		}
		break;
	}
	
	case State::MEDIA: {
		ENetEvent e;
		if (connection.pollEvents(&e) && e.type == ENET_EVENT_TYPE_RECEIVE) {
			PacketView p(e.packet);
			
			if (p.type == Packet::Type::MEDIA) {
				AssetType t = p.d.read<AssetType>();
				usize count = 0;
				
				while (t != AssetType::END) {
					string assetName = p.d.read<string>();
					status->setProp(Gui::Prop::CONTENT,
						status->getStyle<string>(Gui::Prop::CONTENT, "") + ".");
					
					if (t == AssetType::TEXTURE) {
						u16 width = p.d.read<u16>();
						u16 height = p.d.read<u16>();
						
						string data = p.d.read<string>();
						string uncompressed = gzip::decompress(data.data(), data.length());
						
						client.game->textures.addImage(
							assetName, true, u16vec2(width, height),
							reinterpret_cast<u8*>(const_cast<char*>(uncompressed.data())));
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
				status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "")
				                                    + " `c7Done.`c1\nLoading complete. `c0`bJoining world...\n");
				
				state = State::DONE;
				let gameScene = make_unique<GameScene>(client);
				// Only set the scene if the GameScene didn't already redirect to an error scene.
				if (client.scene.isCurrent(this)) client.scene.setScene(std::move(gameScene));
				return;
			}
		}
		break;
	}
	}
	
	status->updateElement();
}

void ConnectScene::handleConnecting() {
	switch (connection.getConnectionStatus()) {
	default:
		throw std::runtime_error("Uncaught connection error.");
	
	case ServerConnection::State::ENET_ERROR:
		throw std::runtime_error("Enet Initialization error.");
	
	case ServerConnection::State::FAILED_CONNECT:
		state = State::FAILED_CONNECT;
		status->setProp(Gui::Prop::CONTENT,
			status->getStyle<string>(Gui::Prop::CONTENT, "") + " `cfFailed to init :(\n");
		break;
	
	case ServerConnection::State::ATTEMPTING_CONNECT:
		connection.processConnecting();
		
		dotsTime += client.getDelta();
		if (dotsTime > 1) {
			dotsTime -= 1;
			status->setProp(Gui::Prop::CONTENT, status->getStyle<string>(Gui::Prop::CONTENT, "") + ".");
		}
		
		break;
	
	case ServerConnection::State::CONNECTED: {
		state = State::PROPERTIES;
		status->setProp(Gui::Prop::CONTENT,
			status->getStyle<string>(Gui::Prop::CONTENT, "") + " `c7Connected!~`c1\n");
		
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
	renderer.enableTexture(&client.game->textures.texture);
	
	root.draw(renderer);
}