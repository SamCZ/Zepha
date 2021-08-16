#pragma once

#include "Player.h"
#include "client/graph/Drawable.h"

#include "util/Target.h"
#include "client/Window.h"
#include "client/gui/GameGui.h"
#include "world/dim/LocalDimension.h"
#include "client/entity/WireframeEntity.h"

class Input;
class Deserializer;
class LuaGuiElement;
class LocalInventory;
class LocalInventoryRefs;
enum class NetPlayerField;

/**
 * The client-side local player. Manages the camera, and renders UI elements.
 */

class LocalPlayer : public virtual DrawableEntity, public Player {
public:
	/** Enum class for identifying player controls in a keyboard-agnostic manner. */
	enum class PlayerControl { FORWARD, LEFT, BACKWARD, RIGHT, JUMP, MOD1, MOD2 };
	
	/**
	 * Creates the local player, and initializes player UI.
	 *
	 * @param game - A reference to the subgame.
	 * @param world - A reference to the world.
	 * @param dim - A reference to player's current dimension.
	 * @param renderer - A reference to the renderer.
	 */
	 
	LocalPlayer(SubgamePtr game, LocalWorld& world, DimensionPtr dim, Renderer& renderer);
	
	/**
	 * Updates the player, processing inputs, physics,
	 * updating the camera, and finding / interacting with the target.
	 *
	 * @param input - The input manager, to pull keyboard events from.
	 * @param delta - The delta time since the last frame.
	 * @param mouseDelta - The mouse offset from the center of the screen.
	 */
	
	void update(Input& input, f64 delta, vec2 mouseDelta);
	
	virtual string getUsername() override;
	
	/** The following setters call Player setters, but updates some rendering-related information. */
	
	virtual void setPos(vec3 pos, bool assert = false) override;
	
	virtual void setLookOffset(vec3 eyeOffset, bool assert = false) override;
	
	virtual void setHandList(const string& list, bool assert = false) override;
	
	virtual void setWieldList(const string& list, bool assert = false) override;
	
	virtual void setWieldIndex(u16 index, bool assert = false) override;
	
	virtual void setDim(DimensionPtr dim) override;
	
	/**
	 * Gets the player's target.
	 * @returns the player's active target.
	 */
	
	Target& getTarget();
	
	/**
	 * Gets the player's inventory.
	 * @returns a reference to the player's inventory.
	 */
	
	virtual InventoryPtr getInventory() override;
	
	/**
	 * Checks if a player is currently in a menu.
	 * @returns if a player is currently in a menu.
	 */
	
	bool isInMenu();
	
	/**
	 * Displays a GUI tree to the player, making it the actively open menu.
	 * @param root - The root GUI element to display.
	 */
	
	void showMenu(sptr<LuaGuiElement> root);
	
	/**
	 * Closes the currently open menu.
	 */
	
	void closeMenu();
	
	/**
	 * Gets the player's HUD element.
	 * @returns the root GUI element of the hud.
	 */
	
	sptr<LuaGuiElement> getHud();
	
	/**
	 * Sets the HUD to the specified GUI tree. The hud does not constitute
	 * a menu and is always displayed unless the hud is hidden.
	 *
	 * @param hud - The root GUI element to display.
	 */
	
	void setHud(sptr<LuaGuiElement> hud);
	
	/**
	 * Sets the visibility state of the HUD.
	 * @param hudVisible - Whether or not the HUD should be visible.
	 */
	
	void setHudVisible(bool hudVisible);
	
	/**
	 * Draws the player's target wireframe and held item.
	 * @param renderer - A reference to the renderer.
	 */
	
	void draw(Renderer& renderer) override;
	
	/**
	 * Draws the player's HUD tree.
	 * @param renderer - A reference to the renderer.
	 */
	
	void drawHud(Renderer& renderer);
	
	/**
	 * Draws the player's menu if they have one open.
	 * @param renderer - A reference to the renderer.
	 */
	
	void drawMenu(Renderer& renderer);
	
	/**
	 * Asserts a player field by sending the specified player packet to the server.
	 * @param packet - The packet to send to the server.
	 */
	
	virtual void assertField(Packet packet) override;
	
	/**
	 * Accepts an assertion packet from the server, updating local properties.
	 * @param d - A deserializer for the assertion packet.
	 */
	
	virtual void handleAssertion(Deserializer& d) override;
	
private:
	
	/**
	 * Checks if a specified player-control was pressed.
	 *
	 * @param input - The input handler to pull key states from.
	 * @param control - The control to check the state of.
	 * @returns a boolean indicating if a specified player control was pressed.
	 */
	 
	bool getKey(Input& input, PlayerControl control);
	
	/**
	 * Moves the player in accordance to the inputs provided.
	 *
	 * @param input - The input handler to pull key states from.
	 * @param delta - The previous frame's delta time.
	 * @param mouseDelta - The offset of the mouse from the center of the screen.
	 */
	
	void updatePhysics(Input& input, f64 delta, vec2 mouseDelta);
	
	/**
	 * Moves the camera to the player's position, and updates the wield model.
	 */
	
	void updateCamera();
	
	/**
	 * Updates the player's wireframe to point at the target.
	 */
	
	void updateWireframe();
	
	/**
	 * Updates the hand and wield item definitions
	 * when their respective lists are changed.
	 */
	
	void updateWieldAndHandItems();
	
	/**
	 * Finds the target under the player's pointer.
	 * @param input - The input handler.
	 */
	
	void findTarget(Input& input);
	
	/**
	 * Checks if the player is trying to interact with a
	 * block or item, and does so if they are.
	 *
	 * @param input - The input handler.
	 * @param delta - The delta time elapsed in the last frame.
	 */
	
	void updateInteract(Input& input, f64 delta);
	
	GameGui gameGui;
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
	
	Window::RCBLock lock;
};

