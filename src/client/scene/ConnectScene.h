//
// Created by aurailus on 11/07/19.
//

#pragma once

#include "Scene.h"

#include "client/Window.h"
#include "client/gui/basic/GuiContainer.h"

class ServerConnection;

class Address;

class ConnectScene : public Scene {
public:
	enum class State {
		CONNECTING,
		FAILED_CONNECT,
		PROPERTIES,
		IDENTIFIER_LIST,
		MODS,
		MEDIA,
		DONE
	};
	
	ConnectScene(Client& state, Address addr);
	
	void update() override;
	
	void draw() override;
	
	void handleConnecting();
	
private:
	State connectState = State::CONNECTING;
	ServerConnection& connection;
	
	GuiContainer components;
	
	double dotsTime = 0;
	
	Window::RCBLock lock;
};
