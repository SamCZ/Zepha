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
    Renderer();
    Renderer(GLint winWidth, GLint winHeight);

    void update();

    void begin();

    void enableWorldShader();
    void draw(Entity* entity);

    void enableGuiShader();
    void drawGui(Entity* entity);

    void end();

    Window* getWindow();
    Camera* getCamera();

    ~Renderer();

    bool resized;
private:
    Window* window;
    Camera* camera;

    glm::mat4* projectionMatrix;
    glm::mat4* orthographicMatrix;

    //LocalWorld Shader
    Shader* worldShader;
    GLint uModel, uProj, uView, uOrtho, uGuiModel;

    Shader* guiShader;
};


#endif //SRC_RENDERER_H
