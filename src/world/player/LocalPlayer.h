#pragma once

#include "Player.h"
#include "client/graph/Drawable.h"

#include "util/Target.h"
#include "client/Window.h"
#include "client/gui/Root.h"
#include "world/dim/LocalDimension.h"
#include "client/entity/WireframeEntity.h"

class Input;
class Deserializer;
class LocalInventory;
class LocalInventoryRefs;
enum class NetPlayerField;
namespace Gui { class BoxElement; }

/**
 * The client-side local player. Manages the camera, and renders UI elements.
 */

class LocalPlayer : public virtual DrawableEntity, public Player {
public:
	/** Enum class for identifying player controls in a keyboard-agnostic manner. */
	enum class PlayerControl { FORWARD, LEFT, BACKWARD, RIGHT, JUMP, MOD1, MOD2 };
	
	/** Creates the local player, and initializes the game Gui. */
	LocalPlayer(SubgamePtr game, LocalWorld& world, DimensionPtr dim, Renderer& renderer);
	
	/** Updates the player, processing inputs, and physics. */
	void update(f64 delta, vec2 mouseDelta);
	
	virtual string getUsername() override;
	
	/* The following call Player setters, but updates some rendering-related information. */
	
	virtual void setPos(vec3 pos, bool assert = false) override;
	
	virtual void setLookOffset(vec3 eyeOffset, bool assert = false) override;
	
	virtual void setHandList(const string& list, bool assert = false) override;
	
	virtual void setWieldList(const string& list, bool assert = false) override;
	
	virtual void setWieldIndex(u16 index, bool assert = false) override;
	
	virtual void setDim(DimensionPtr dim) override;
	
	/** Gets the player's current target. */
	Target& getTarget();
	
	/** Gets the player's inventory. */
	virtual InventoryPtr getInventory() override;
	
	/** Checks if a player is currently in a menu. */
	bool isInMenu();
	
	/** Displays a menu to the player, replacing any other menu. */
	void showMenu(sptr<Gui::Element> menu);
	
	/** Closes the currently open menu. */
	void closeMenu();
	
	/** Returns the Gui root. */
	Gui::Root& getRoot();
	
	/** Gets the debug element. */
	sptr<Gui::Element> getDebug();
	
	/** Gets the hud element. */
	sptr<Gui::Element> getHud();
	
	/** Sets the hud to the specified element. */
	void setHud(sptr<Gui::Element> hud);
	
	/** Sets whether or not the hud should be visible. */
	void setHudVisible(bool visible);
	
	/** Draws the player's target wireframe and held item. */
	void draw(Renderer& renderer) override;
	
	/** Draws the player's hud. */
	void drawHud(Renderer& renderer);
	
	/** Draws the currently open menu, if there is one. */
	void drawMenu(Renderer& renderer);
	
	/** Asserts a player field by sending the specified player packet to the server. */
	virtual void assertField(Packet packet) override;
	
	/** Accepts an assertion packet from the server, updating local properties. */
	virtual void handleAssertion(Deserializer& d) override;
	
private:
	
	/** Checks if a specified player-control was pressed. */
	bool getKey(PlayerControl control);
	
	/** Moves the player in accordance to the inputs provided. */
	void updatePhysics(f64 delta, vec2 mouseDelta);
	
	/** Moves the camera to the player's position, and updates the wield model. */
	void updateCamera();
	
	/** Updates the player's wireframe to point at the target. */
	void updateWireframe();
	
	/** Updates the hand and wield item definitions when their respective lists are changed. */
	void updateWieldAndHandItems();
	
	/** Finds the target under the player's pointer. */
	void findTarget();
	
	/** Checks if the player is trying to interact with a block or item, and does so if they are. */
	void updateInteract(f64 delta);
	
	/** The Gui root. */
	Gui::Root root;
	
	/** Element roots for the hud and menu, respectively. */
	sptr<Gui::BoxElement> hud, menu, debug;
	
	/** A reference to the renderer. */
	Renderer& renderer;
	
	/** The player's current target. */
	Target target;
	
	/** The time remaining until the next hit. */
	f64 breakTime = 0;
	
	/** The interval at which the currently wielded tool can hit. */
	f64 breakInterval = 0;
	
	/** The targeted wireframe entity. */
	WireframeEntity wireframe;
	
	/** A wrapper for the wield-item, translated to the bottom right of the player's viewport. */
	DrawableEntity handModel;
	
	/** The actual wield-item model, set to the currently held item. */
	DrawableEntity handItemModel;
};

