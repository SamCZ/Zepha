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
	
	/**
	 * Creates a client window and starts the main event loop.
	 * Initially opens to the main menu.
	 * @param window - The dimensions for the created window.
	 */
	
	explicit Client(ivec2 window);
	
	/** Returns the last frame's delta time. */
	f64 getDelta();

	/**
	 * Starts a local server and connects to it.
	 * @throws runtime_error if a local server is already running.
	 * @param subgame - The subgame for the local server to run.
	 */
	 
	void startLocalServer(const string& subgame);
	
	Renderer renderer;
	SceneManager scene;
	ServerConnection connection{};
	
	sptr<LocalSubgame> game = make_shared<LocalSubgame>("../assets/textures");
	
private:
	
	/**
	 * The main event loop. Polls GLFW, and updates the scene and the renderer.
	 * Will be called by the Client constructor until render.window.shouldClose() returns true.
	 */
	 
	void loop();
	
	sptr<LocalServerInstance> localServer = nullptr;
	
	f64 delta = 0;
	f64 timeElapsed = 0;
};

