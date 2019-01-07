//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENT_H
#define ZEUS_CLIENT_H

#include "../game/GameInstance.h"
#include "../engine/graphics/Renderer.h"

class Client {
public:
    Client();
    Client(int width, int height);

    void start();

    ~Client();

private:
    Renderer* renderer;
    GameInstance* scene;

    double fps = 0.0;

    void loop();

    void cleanup();

    double timeElapsed = 0.0f;
};


#endif //ZEUS_CLIENT_H
