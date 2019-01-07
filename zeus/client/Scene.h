//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_SCENE_H
#define ZEUS_SCENE_H

#include "ClientState.h"

class Scene {
public:
    explicit Scene(ClientState* state) {
        this->state = state;
    };

    virtual void update() {};

    virtual void draw() {};

    virtual void cleanup() {};

    ClientState* state;
};


#endif //ZEUS_SCENE_H
