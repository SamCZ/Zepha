
#include <iostream>

#include "Client.h"

#include "../util/Timer.h"
#include "LocalServerInstance.h"

#include "scene/GameScene.h"
#include "scene/ConnectScene.h"
#include "scene/MainMenuScene.h"


/**
 * Creates a client window and starts the main event loop.
 * Initially opens to the main menu.
 * @param window - The dimensions for the created window.
 */

Client::Client(glm::ivec2 window) :
	renderer(window) {
	
	std::cout << Log::info << "Starting Zepha Client." << Log::endl;
	
	scene.setScene(std::make_unique<MainMenuScene>(*this));
	while (!renderer.window.shouldClose()) loop();
}


/**
 * Get the last frame's delta time.
 * @returns the delta time.
 */

double Client::getDelta() {
	return delta;
}


/**
 * Starts a local server and connects to it.
 * @throws runtime_error if a local server is already running.
 * @param subgame - The subgame for the local server to run.
 */

void Client::startLocalServer(const std::string& subgame) {
	//TODO: Implement Local Server
//    localServer = std::make_shared<LocalServerInstance>(executablePath, addr.port, state.subgame);
//    localServer->start();
	
	scene.setScene(std::make_unique<ConnectScene>(*this, Address{ "127.0.0.1", Address::DEFAULT_PORT }));
}


/**
 * The main event loop. Polls GLFW, and updates the scene and the renderer.
 * Will be called by the Client constructor until render.window.shouldClose() returns true.
 */

void Client::loop() {
	double now = glfwGetTime();
	delta = now - timeElapsed;
	timeElapsed = now;
	
	glfwPollEvents();
	
	scene.update();
	renderer.update(delta);
}