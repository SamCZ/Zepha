#pragma once

#include "Scene.h"

#include "client/Window.h"
#include "client/gui/Root.h"

class Address;
class ServerConnection;
namespace Gui { class TextElement; };

class ConnectScene : public Scene {
public:
	
	/** Represents the loading state. */
	enum class State {
		CONNECTING,
		FAILED_CONNECT,
		PROPERTIES,
		IDENTIFIER_LIST,
		MODS,
		MEDIA,
		DONE
	};
	
	/**
	 * Initializes a connection to a remote server, displays download progress.
	 * Starts the GameScene once the assets have been downloaded.
	 */
	 
	ConnectScene(Client& state, Address addr);
	
	void update() override;
	
	void draw() override;
	
	/** Handles displaying the connection progress. */
	void handleConnecting();
	
private:
	ServerConnection& connection;
	State state = State::CONNECTING;
	
	Gui::Root root;
	sptr<Gui::TextElement> status;
	
	f64 dotsTime = 0;
	u32 modsFound = 0;
};
