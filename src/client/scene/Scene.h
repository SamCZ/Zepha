#pragma once

class Client;

/**
 * Scene abstract class, provides base scene methods for use by SceneManager.
 * Stores a reference to the client, for use by children.
 */

class Scene {
public:
	/** Creates the scene. */
	explicit Scene(Client& client) : client(client) {}
	
	/** Called every game loop to update internal state. */
	virtual void update() {};
	
	/** Called every game loop to draw objects. */
	virtual void draw() {};
	
	/** Called when the scene will be destroyed. */
	virtual void cleanup() {};
	
	virtual ~Scene() = default;
	
	/** A reference to the client that created the scene. */
	Client& client;
};

