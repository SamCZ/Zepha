//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_SCENEMANAGER_H
#define ZEUS_SCENEMANAGER_H

#include "Scene.h"
#include <memory>

class SceneManager {
public:
    SceneManager() = default;

    void setScene(std::unique_ptr<Scene> scene);
    Scene& getScene();

    void update();
    void cleanupScene();

    ~SceneManager();
private:
    std::unique_ptr<Scene> scene = nullptr;
};


#endif //ZEUS_SCENEMANAGER_H
