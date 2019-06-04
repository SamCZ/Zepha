//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_SCENE_H
#define ZEUS_SCENE_H

#include "../../../game/ClientState.h"
#include <iostream>

class Scene {
public:
    explicit Scene(ClientState* state) {
        this->state = state;
    };

    virtual void update() {};

    virtual void draw() {};

    virtual void cleanup() {};

    virtual ~Scene() = default;

    ClientState* state;
};


#endif //ZEUS_SCENE_H
