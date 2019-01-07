//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_SCENEMANAGER_H
#define ZEUS_SCENEMANAGER_H


#include "Scene.h"
#include "ClientState.h"

class SceneManager {
public:
    SceneManager();

    void setScene(Scene* scene);
    Scene* getScene();

    void update();

    void cleanupScene();

    ~SceneManager();
private:
    Scene* scene;
};


#endif //ZEUS_SCENEMANAGER_H
