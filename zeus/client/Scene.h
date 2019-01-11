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

    virtual void update() {
        std::cerr << "State doesn't define update method!" << std::endl;
    };

    virtual void draw() {
        std::cerr << "State doesn't define draw method!" << std::endl;
    };

    virtual void cleanup() {
        std::cerr << "State doesn't define cleanup method!" << std::endl;
    };

    ClientState* state;
};


#endif //ZEUS_SCENE_H
