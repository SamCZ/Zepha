/*
 * Scene abstract superclass, provides base scene methods for use by SceneManager.
 * Stores a reference to the client, for children.
 *
 * - Auri, 03/11/20
 */

#pragma once

class Client;

class Scene {
public:
    explicit Scene(Client& client) : client(client) {}

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void cleanup() = 0;

    virtual ~Scene() = default;

    Client& client;
};

