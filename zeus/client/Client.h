//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENT_H
#define ZEUS_CLIENT_H

#include "../engine/graphics/Renderer.h"
#include "../engine/Timer.h"
#include "SceneManager.h"
#include "ClientState.h"

class Client {
public:
    Client();
    Client(int width, int height);

    void start();

    ~Client();

private:
    void loop();

    void cleanup();

    Renderer* renderer;

    ClientState* state;
    SceneManager sceneManager;

    double timeElapsed = 0.0f;
};


#endif //ZEUS_CLIENT_H
