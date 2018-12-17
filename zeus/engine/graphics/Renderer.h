//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#include "Shader.h"
#include "../Window.h"
#include "../Camera.h"
#include "../Entity.h"
#include <ext.hpp>

class Renderer {
public:
    Renderer(GLint winWidth, GLint winHeight);

    void update();

    void begin();
    void draw(Entity* entity);
    void end();

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();
private:
    Window* window;
    Camera* camera;

    glm::mat4 projectionMatrix;

    //World Shader
    Shader* shader;
};


#endif //SRC_RENDERER_H
