#include <iostream>

#include "Client.h"

#include "util/Types.h"
#include "scene/ConnectScene.h"
#include "scene/MainMenuScene.h"

Client::Client(ivec2 window) :
	renderer(window) {
	
	std::cout << Log::info << "Starting Zepha Client." << Log::endl;
	
	scene.setScene(make_unique<MainMenuScene>(*this));
	while (!renderer.window.shouldClose()) loop();
}

f64 Client::getDelta() {
	return delta;
}

void Client::startLocalServer(const string& subgame) {
//    TODO: Implement Local Server
//    localServer = std::make_shared<LocalServerInstance>(executablePath, addr.port, state.subgame);
//    localServer->start();
	
	scene.setScene(make_unique<ConnectScene>(*this, Address { "127.0.0.1", Address::DEFAULT_PORT }));
}

void Client::loop() {
	f64 now = glfwGetTime();
	delta = now - timeElapsed;
	timeElapsed = now;
	
	glfwPollEvents();
	
	scene.update();
	renderer.update(delta);
	game->textures.update();
	
	renderer.swapBuffers();
}