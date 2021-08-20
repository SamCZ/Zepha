#pragma once

#include "Scene.h"

#include "util/Types.h"
#include "client/Window.h"
#include "client/gui/Root.h"
#include "client/menu/SubgameDef.h"
#include "client/menu/MenuSandbox.h"

class Client;

/**
 * Displays the main menu, which has a list of installed subgames which can be selected and played.
 * Each subgame can have its own menu, which is displayed in the sandbox area.
 */

class MainMenuScene : public Scene {
public:
	
	explicit MainMenuScene(Client& client);
	
	void update() override;
	
	void draw() override;
	
private:
	
	/** Find valid subgames in the subgames folder. */
	void findSubgames();
	
	/** Provides the API for menu mods. */
//	MenuSandbox sandbox;
	Gui::Root root;

	/** A list of found subgames. */
	vec<SubgameDef> subgames;
	
	/** A reference to the currently selected subgame. */
	SubgameDef* selectedSubgame = nullptr;
};

