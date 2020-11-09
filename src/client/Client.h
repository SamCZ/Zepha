/*
 * Main Client object. Contains important properties, such as the renderer and scene manager.
 * Also manages the local server, if there is one.
 *
 * - Auri, 03/11/20
 */

#pragma once

#include "util/net/Address.h"
#include "game/LocalSubgame.h"
#include "client/graph/Renderer.h"
#include "client/scene/SceneManager.h"
#include "client/conn/ServerConnection.h"

class LocalServerInstance;

class Client {
	public:
	Client(const Client& o) = delete;
	
	explicit Client(glm::ivec2 window);
	
	double getDelta();
	
	void startLocalServer(const std::string& subgame);
	
	Renderer renderer;
	SceneManager scene;
	ServerConnection connection{};
	
	std::shared_ptr<LocalSubgame> game = std::make_shared<LocalSubgame>("../assets/textures");
	
	private:
	void loop();
	
	std::shared_ptr<LocalServerInstance> localServer = nullptr;
	
	double delta = 0;
	double timeElapsed = 0;
};

