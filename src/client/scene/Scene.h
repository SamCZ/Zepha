//
// Created by aurailus on 06/01/19.
//

#pragma once

class ClientState;

class Scene {
public:
    explicit Scene(ClientState& state) : state(state) {}

    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void cleanup() = 0;

    virtual ~Scene() = default;

    ClientState& state;
};

